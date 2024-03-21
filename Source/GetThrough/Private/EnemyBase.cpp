// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "PlayerBase.h"
#include "Components/CapsuleComponent.h"

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate OnHitDelegate;
	OnHitDelegate.BindUFunction(this, FName("OnHit"));
	GetCapsuleComponent()->OnComponentHit.AddUnique(OnHitDelegate);
}

void AEnemyBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const noexcept
{
	APlayerBase* PlayerPawn = Cast<APlayerBase>(OtherActor);
	if (PlayerPawn && !PlayerPawn->IsDead())
	{
		PlayerPawn->Die();
	}
}

void AEnemyBase::Die() noexcept
{
	bIsDead = true;
	Cast<AAIController>(GetController())->GetBrainComponent()->StopLogic("Controlled pawn died.");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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