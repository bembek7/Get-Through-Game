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

	void ApplyDamage(float Damage) noexcept;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const noexcept;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const noexcept;

private:	
	void Die() noexcept;

public:

protected:
	UPROPERTY(BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(BlueprintReadWrite)
	float ChaseSpeed = 600.f;

	UPROPERTY(BlueprintReadWrite)
	float RoamSpeed = 50.f;

private:
	bool bIsDead = false;
	
};
