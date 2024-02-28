// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "PlayerControllerBase.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayerBase::APlayerBase() noexcept
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(GetMesh(), FName("hand_r"));

	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("Torch"));
	Torch->SetupAttachment(GetCapsuleComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
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
