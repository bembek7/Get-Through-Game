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

	FVector GetShootingStartLocation() const;

	void Die();

	void TurnTorchOff();

	bool IsDead() const;

	float CalculateAimPitch() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:

public:

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	float AimPitch = 0.f;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* Torch;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Gun;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

private:
	bool bIsDead = false;
};
