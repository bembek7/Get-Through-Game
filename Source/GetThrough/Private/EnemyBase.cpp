// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "PlayerBase.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate OnHitDelegate;
	OnHitDelegate.BindUFunction(this, FName("OnHit"));
	GetCapsuleComponent()->OnComponentHit.AddUnique(OnHitDelegate);

	bReplicates = true;
}

void AEnemyBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const
{
	APlayerBase* const PlayerPawn = Cast<APlayerBase>(OtherActor);
	if (PlayerPawn && !PlayerPawn->IsDead())
	{
		PlayerPawn->Die();
	}
}

void AEnemyBase::OnHealthUpdate()
{
	if (Health <= 0)
	{
		Die();
	}
}

void AEnemyBase::SetHealth(const float NewHealth)
{
	if (HasAuthority())
	{
		Health = NewHealth;
		OnHealthUpdate();
	}
}

void AEnemyBase::Die()
{
	bIsDead = true;
	if (AAIController* const EnemyController = Cast<AAIController>(GetController()))
	{
		EnemyController->GetBrainComponent()->StopLogic("Controlled pawn died.");
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetEnableGravity(false);
	GetMesh()->SetEnableGravity(false);
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
			SetHealth(Health - Damage);
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

	DOREPLIFETIME(AEnemyBase, Health);
	DOREPLIFETIME(AEnemyBase, ChaseSpeed);
	DOREPLIFETIME(AEnemyBase, RoamSpeed);
	DOREPLIFETIME(AEnemyBase, bIsDead);
}