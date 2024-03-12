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

	// Sets default values for this pawn's properties
	AEnemyBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyDamage(float Damage) noexcept;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const noexcept;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpuls, const FHitResult& Hit);

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
