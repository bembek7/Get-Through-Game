// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Die() noexcept
{
	bIsDead = true;
	Cast<AAIController>(GetController())->GetBrainComponent()->StopLogic("Controlled pawn died.");
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ApplyDamage(float Damage) noexcept
{
	if (!bIsDead)
	{
		Health -= Damage;
		if (Health <= 0)
		{
			Die();
		}
	}
}

bool AEnemyBase::IsDead() const noexcept
{
	return bIsDead;
}

