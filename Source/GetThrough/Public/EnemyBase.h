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

	void ApplyDamage(const float Damage) noexcept;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const noexcept;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit) const noexcept;

	void OnHealthUpdate();

	UFUNCTION(BlueprintCallable)
	void SetHealth(const float NewHealth);
	
private:
	void Die() noexcept;

public:

protected:
	UPROPERTY(Replicated)
	float Health = 100.f;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float ChaseSpeed = 600.f;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float RoamSpeed = 50.f;

private:
	UPROPERTY(Replicated)
	bool bIsDead = false;
};
