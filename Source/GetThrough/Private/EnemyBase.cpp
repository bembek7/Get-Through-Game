// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
}

void AEnemyBase::SetHealth(const float NewHealth)
{
	if (HasAuthority())
	{
		CurrentHealth = NewHealth;
		OnHealthUpdate();
	}
}

void AEnemyBase::OnHealthUpdate()
{
	if (CurrentHealth <= 0)
	{
		Die();
	}
}

void AEnemyBase::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AEnemyBase::Die()
{
	bIsDead = true;
	if (AAIController* const EnemyController = Cast<AAIController>(GetController()))
	{
		EnemyController->GetBrainComponent()->StopLogic("Controlled pawn died.");
	}
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::ApplyDamage(const float Damage)
{
	if (HasAuthority())
	{
		if (!bIsDead)
		{
			SetHealth(CurrentHealth - Damage);
		}
	}
}

bool AEnemyBase::IsDead() const
{
	return bIsDead;
}

void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyBase, CurrentHealth);
	DOREPLIFETIME(AEnemyBase, bIsDead);
}