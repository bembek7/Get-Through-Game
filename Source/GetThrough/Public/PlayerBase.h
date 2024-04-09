// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "PlayerBase.generated.h"

UCLASS()
class GETTHROUGH_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerBase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetShotBeamStartLocation() const;

	void Die();

	void TurnTorchOff();

	bool IsDead() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	float GetAimPitch() const;

	FVector GetShootingTraceStartPointLocation() const;
	FVector GetShootingDirection() const;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_PlayGunshotSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_DrawBulletTrace(const FVector& BeamStart, const FVector& BeamDirection) const;

protected:
	virtual void BeginPlay() override;

private:

public:

protected:
	UPROPERTY(EditDefaultsOnly)
	USoundBase* GunshotSound;

	UPROPERTY(EditDefaultsOnly)
	float GunshotSoundRange = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* BulletTraceSystem;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* Torch;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Gun;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ShootingTracesStartPoint;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

private:
	bool bIsDead = false;
};
