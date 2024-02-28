// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"


#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GETTHROUGH_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupInput(class UInputComponent* PlayerInputComponent) noexcept;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(Category = "Input Response")
	void Walk(const FInputActionValue& IAValue) noexcept;

	UFUNCTION(Category = "Input Response")
	void Shoot() noexcept;

	void RotatePlayerToFaceTheCursor(float DeltaTime) noexcept;

	void PlayGunshotSound(const FVector& GunLocation) const noexcept;
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAWalk;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAShoot;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* GunshotSound;

private:
	FVector LastRecordedMouseLocation;
	FRotator LastRecordedRotationWithMouseInViewport;
};
