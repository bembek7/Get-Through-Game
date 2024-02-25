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
    FVector MouseLocation;
    FVector MouseDirection;
    if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
    {
        APawn* PlayerPawn = GetPawn();
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FRotator PlayerToMouseRotation = PlayerPawn->GetActorRotation();
        
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
        float RotatingSpeed = 100.f;
        Cast<ACharacter>(GetPawn())->FaceRotation(NewRotation);
    }
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
