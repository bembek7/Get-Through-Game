// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "ZombieBase.generated.h"

USTRUCT(BlueprintType)
struct FZombieType
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UAnimInstance>AnimationBlueprint;
	UPROPERTY(BlueprintReadOnly)
	float RoamSpeed = 200.f;
	UPROPERTY(BlueprintReadOnly)
	float ChaseSpeed = 600.f;
	UPROPERTY()
	float MaxHealth = 100.f;
};
/**
 *
 */
UCLASS()
class GETTHROUGH_API AZombieBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnRep_ZombieType();

	void OnZombieTypeChanges();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const;

	FZombieType PickRandomZombieType() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAnimInstance>>FastZombieAnimationBlueprints;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAnimInstance>>RegularZombieAnimationBlueprints;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAnimInstance>>CrawlingZombieAnimationBlueprints;

	UPROPERTY(ReplicatedUsing = OnRep_ZombieType, BlueprintReadOnly)
	FZombieType ZombieType;
};
