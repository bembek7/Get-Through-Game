// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBase.h"
#include "PlayerControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "GSBase.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimuli Source"));

	SpringArmForCollision = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm For Collision"));
	SpringArmForCollision->SetupAttachment(GetCapsuleComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(SpringArmForCollision);

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

	if (IsLocallyControlled())
	{
		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = false;
		}
	}
	else
	{
		if (SpringArm)
		{
			SpringArm->bUsePawnControlRotation = true;
		}
	}

	if (!HasAuthority() && !IsLocallyControlled())
	{
		if (SpringArmForCollision)
		{
			SpringArm->bUsePawnControlRotation = false;
		}
	}
}

void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerControllerBase* const PlayerController = Cast<APlayerControllerBase>(GetController()))
	{
		PlayerController->SetupInput(PlayerInputComponent);
	}
}

FVector APlayerBase::GetShotBeamStartLocation() const
{
	return (Gun) ? Gun->GetSocketLocation(FName("Muzzle")) : FVector();
}

void APlayerBase::Die()
{
	bIsDead = true;
	if (APlayerControllerBase* const PlayerController = Cast<APlayerControllerBase>(GetController()))
	{
		PlayerController->ControlledPlayerDied();
	}
	if (AGSBase* const GameState = Cast<AGSBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		GameState->AnyPlayerDied();
	}
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	bUseControllerRotationYaw = false;
	GetWorldTimerManager().SetTimer(DestroyAfterDeathTimer, [this]() { Destroy(); }, 1.f, false);
}

void APlayerBase::TurnTorchOff()
{
	Torch->SetVisibility(false);
}

bool APlayerBase::IsDead() const
{
	return bIsDead;
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

float APlayerBase::GetAimPitch() const
{
	return Camera->GetComponentRotation().Pitch;
}

FVector APlayerBase::GetShootingTraceStartPointLocation() const
{
	return (Camera) ? Camera->GetComponentLocation() : FVector();
}

FVector APlayerBase::GetShootingDirection() const
{
	return (Camera) ? Camera->GetForwardVector() : FVector();
}

void APlayerBase::NetMulticast_PlayGunshotSound_Implementation()
{
	if (GunshotSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSound, GetShotBeamStartLocation());
	}
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.f, this, GunshotSoundRange, FName("Gunshot"));
}

void APlayerBase::NetMulticast_DrawBulletTrace_Implementation(const FVector& BeamStart, const FVector& BeamDirection) const
{
	UNiagaraComponent* const BulletTraceEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTraceSystem, BeamStart);
	BulletTraceEffect->SetVectorParameter(FName("BeamEnd"), BeamDirection);
}