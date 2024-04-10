// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieBase.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBase.h"

void AZombieBase::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetIsReplicated(true);
	if (HasAuthority())
	{
		ZombieType = PickRandomZombieType();
		OnZombieTypeChanges();
	}

	FScriptDelegate OnHitDelegate;
	OnHitDelegate.BindUFunction(this, FName("OnHit"));
	GetCapsuleComponent()->OnComponentHit.AddUnique(OnHitDelegate);
}

void AZombieBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const
{
	APlayerBase* const PlayerPawn = Cast<APlayerBase>(OtherActor);
	if (PlayerPawn && !PlayerPawn->IsDead())
	{
		PlayerPawn->Die();
	}
}

void AZombieBase::OnRep_ZombieType()
{
	OnZombieTypeChanges();
}

void AZombieBase::OnZombieTypeChanges()
{
	SetHealth(ZombieType.MaxHealth);
	if (ZombieType.AnimationBlueprint)
	{
		GetMesh()->SetAnimInstanceClass(ZombieType.AnimationBlueprint);
	}
}

FZombieType AZombieBase::PickRandomZombieType() const
{
	FZombieType RandomZombieType;
	int32 RandomizingResult = UKismetMathLibrary::RandomInteger(FastZombieAnimationBlueprints.Num() + RegularZombieAnimationBlueprints.Num() + CrawlingZombieAnimationBlueprints.Num());
	if (RandomizingResult < FastZombieAnimationBlueprints.Num())
	{
		RandomZombieType.AnimationBlueprint = FastZombieAnimationBlueprints[RandomizingResult];
		RandomZombieType.MaxHealth = 50.f;
		RandomZombieType.ChaseSpeed = 700.f;
		RandomZombieType.RoamSpeed = 300.f;
		return RandomZombieType;
	}
	RandomizingResult -= FastZombieAnimationBlueprints.Num();
	if (RandomizingResult < RegularZombieAnimationBlueprints.Num())
	{
		RandomZombieType.AnimationBlueprint = RegularZombieAnimationBlueprints[RandomizingResult];
		RandomZombieType.MaxHealth = 120.f;
		RandomZombieType.ChaseSpeed = 500.f;
		RandomZombieType.RoamSpeed = 150.f;
		return RandomZombieType;
	}
	RandomizingResult -= RegularZombieAnimationBlueprints.Num();
	RandomZombieType.AnimationBlueprint = CrawlingZombieAnimationBlueprints[RandomizingResult];
	RandomZombieType.MaxHealth = 170.f;
	RandomZombieType.ChaseSpeed = 300.f;
	RandomZombieType.RoamSpeed = 50.f;
	return RandomZombieType;
}

void AZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZombieBase, ZombieType);
}