// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "PlayerControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerBase::APlayerBase() noexcept
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	//Gun->SetupAttachment(SpringArm);
	Gun->SetupAttachment(GetMesh(), FName("hand_r"));
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());
	if (PlayerController)
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
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetController());
	if (PlayerController)
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
