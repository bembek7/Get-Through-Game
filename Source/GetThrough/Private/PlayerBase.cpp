// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBase.h"
#include "PlayerControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

APlayerBase::APlayerBase() noexcept
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("Torch"));
	Torch->SetupAttachment(Camera);

	SpringArm->bDoCollisionTest = true;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(GetMesh(), FName("hand_r"));

	bReplicates = true;
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		AimPitch = CalculateAimPitch();
	}
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController()))
	{
		PlayerController->SetupInput(PlayerInputComponent);
	}
}

FVector APlayerBase::GetShootingStartLocation() const noexcept
{
	return Gun->GetSocketLocation(FName("Muzzle"));
}

void APlayerBase::Die() noexcept
{
	bIsDead = true;
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController()))
	{
		PlayerController->PlayerDied();
	}
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bUseControllerRotationYaw = false;
}

void APlayerBase::TurnTorchOff() noexcept
{
	Torch->SetVisibility(false);
}

bool APlayerBase::IsDead() const noexcept
{
	return bIsDead;
}

float APlayerBase::CalculateAimPitch() const noexcept
{
	const float BaseAimRotation = GetBaseAimRotation().Pitch;
	return (BaseAimRotation <= 90) ? BaseAimRotation : BaseAimRotation - 360;
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerBase, AimPitch);
}
