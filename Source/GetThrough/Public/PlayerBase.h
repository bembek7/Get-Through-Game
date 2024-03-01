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
	// Sets default values for this character's properties
	APlayerBase() noexcept;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetShootingStartLocation() const noexcept;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:
	

protected:
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

};
