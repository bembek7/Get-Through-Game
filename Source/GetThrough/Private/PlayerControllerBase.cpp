// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
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
}

void APlayerControllerBase::Tick(float DeltaTime)
{
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
    }
}

void APlayerControllerBase::Walk(const FInputActionValue& IAValue) noexcept
{
    const FVector2D MoveVector = IAValue.Get<FVector2D>();
    float XAxis = MoveVector.X;
    float YAxis = MoveVector.Y;
    APawn* PlayerPawn = GetPawn();
    PlayerPawn->AddMovementInput(FVector(0, 1, 0), XAxis);
    PlayerPawn->AddMovementInput(FVector(1, 0, 0), YAxis);
}

void APlayerControllerBase::Shoot() noexcept
{
    UE_LOG(LogTemp, Warning, TEXT("Shoot"));
}

void APlayerControllerBase::RotatePlayerToFaceTheCursor(float DeltaTime) noexcept
{
    FVector MouseLocation;
    FVector MouseDirection;
    APawn* PlayerPawn = GetPawn();

    if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        // Find intersect point with plane originating on actor 
        FVector ActorLocation = PlayerPawn->GetActorLocation();
        FVector EndLocation = FMath::LinePlaneIntersection(
            MouseLocation,
            MouseLocation + (MouseDirection * 10000.f),
            ActorLocation,
            FVector{ 0.f, 0.f, 1.f }
        );

        // Change actor's yaw rotation
        FRotator NewRotation = PlayerPawn->GetActorRotation();
        NewRotation.Yaw = (EndLocation - ActorLocation).Rotation().Yaw;
        Cast<ACharacter>(PlayerPawn)->FaceRotation(NewRotation, DeltaTime);
        
        LastRecordedRotationWithMouseInViewport = NewRotation;
    }
    else
    {
        Cast<ACharacter>(PlayerPawn)->FaceRotation(LastRecordedRotationWithMouseInViewport, DeltaTime);
    }
}
