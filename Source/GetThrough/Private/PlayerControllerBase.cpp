// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBase.h"
#include "Perception/AISense_Hearing.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

    MapWidget = CreateWidget<UUserWidget>(this, MapWidgetClass);
    MapWidget->AddToPlayerScreen();
    MapWidget->SetVisibility(ESlateVisibility::Collapsed);

    DeathWidget = CreateWidget<UUserWidget>(this, DeathWidgetClass);
    DeathWidget->AddToPlayerScreen();
    DeathWidget->SetVisibility(ESlateVisibility::Collapsed);

    if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (!InputMappingContext.IsNull())
            {
                InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 2);
            }
        }
    }
    bShowMouseCursor = true;
    CurrentMouseCursor = EMouseCursor::Crosshairs;
    auto InputMode = FInputModeGameAndUI();
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
}

void APlayerControllerBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RotatePlayerToFaceTheCursor(DeltaTime);
}

void APlayerControllerBase::SetupInput(UInputComponent* PlayerInputComponent) noexcept
{
    if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Binding inputs
        if (IAWalk)
        {
            PlayerEnhancedInputComponent->BindAction(IAWalk, ETriggerEvent::Triggered, this, &APlayerControllerBase::Walk);
        }
        if (IAShoot)
        {
            PlayerEnhancedInputComponent->BindAction(IAShoot, ETriggerEvent::Started, this, &APlayerControllerBase::Shoot);
        }
        if (IAMap)
        {
            PlayerEnhancedInputComponent->BindAction(IAMap, ETriggerEvent::Started, this, &APlayerControllerBase::ShowHideMap);
        }
    }
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
    return TeamId;
}

void APlayerControllerBase::PlayerDied() noexcept
{
    SetInputMode(FInputModeUIOnly());
    Cast<APlayerBase>(GetPawn())->TurnTorchOff();
    if (DeathWidget)
    {
        DeathWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void APlayerControllerBase::Walk(const FInputActionValue& IAValue) noexcept
{
    const FVector2D MoveVector = IAValue.Get<FVector2D>();
    const float XAxis = MoveVector.X;
    const float YAxis = MoveVector.Y;
    APawn* PlayerPawn = GetPawn();
    PlayerPawn->AddMovementInput(FVector(0, 1, 0), XAxis);
    PlayerPawn->AddMovementInput(FVector(1, 0, 0), YAxis);
}

void APlayerControllerBase::Shoot() noexcept
{
    APlayerBase* PlayerPawn = Cast<APlayerBase>(GetPawn());
    FHitResult Hit;
    FVector GunLocation = PlayerPawn->GetShootingStartLocation();
    FVector MouseLocation = FVector(LastRecordedMouseLocation.X, LastRecordedMouseLocation.Y, GunLocation.Z);
    FVector TraceDirection = (MouseLocation - GunLocation).GetSafeNormal(1.f);
    FVector TraceEnd = GunLocation + TraceDirection * 2000.f;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    GetWorld()->LineTraceSingleByChannel(Hit, GunLocation, TraceEnd, ECollisionChannel::ECC_Camera, QueryParams);

    if (Hit.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), GunLocation, Hit.ImpactPoint, FColor::Yellow, false, 0.2f, 0, 2.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GunLocation, TraceEnd, FColor::Yellow, false, 0.2f, 0, 2.0f);
    }

    AEnemyBase* EnemyHit = Cast<AEnemyBase>(Hit.GetActor());
    if (EnemyHit)
    {
        EnemyHit->ApplyDamage(GunDamage);
    }

    PlayGunshotSound(GunLocation);
}

void APlayerControllerBase::ShowHideMap() noexcept
{
    if (MapWidget->IsVisible())
    {
        HideMap();
    }
    else
    {   
        ShowMap();
    }
}

void APlayerControllerBase::ShowMap() noexcept
{
    SetPause(true);
    MapWidget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerControllerBase::HideMap() noexcept
{
    SetPause(false);
    MapWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerControllerBase::RotatePlayerToFaceTheCursor(float DeltaTime) noexcept
{
    FVector MouseLocation;
    FVector MouseDirection;
    APawn* PlayerPawn = GetPawn();

    if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection) && !IsPaused())
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        FVector EndLocation = FMath::LinePlaneIntersection(
            MouseLocation,
            MouseLocation + (MouseDirection * 10000.f),
            PlayerLocation,
            FVector{ 0.f, 0.f, 1.f }
        );

        // Change actor's yaw rotation
        FRotator NewRotation = PlayerPawn->GetActorRotation();
        NewRotation.Yaw = (EndLocation - PlayerLocation).Rotation().Yaw;

        Cast<ACharacter>(PlayerPawn)->FaceRotation(NewRotation, DeltaTime);
        
        LastRecordedMouseLocation = EndLocation;
        LastRecordedRotationWithMouseInViewport = NewRotation;
    }
    else
    {
        Cast<ACharacter>(PlayerPawn)->FaceRotation(LastRecordedRotationWithMouseInViewport, DeltaTime);
    }
}

void APlayerControllerBase::PlayGunshotSound(const FVector& GunLocation) const noexcept
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSound, GunLocation);
    APawn* PlayerPawn = GetPawn();
    UAISense_Hearing::ReportNoiseEvent(GetWorld(), PlayerPawn->GetActorLocation(), 1.f, PlayerPawn, GunshotSoundRange, FName("Gunshot"));
}
