// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class GETTHROUGH_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	void ApplyDamage(const float Damage);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	void OnHealthUpdate();

	UFUNCTION(BlueprintCallable)
	void SetHealth(const float NewHealth);

private:
	UFUNCTION()
	void OnRep_CurrentHealth();

	void Die();

public:

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth = 100.f;

private:
	UPROPERTY(Replicated)
	bool bIsDead = false;
};
