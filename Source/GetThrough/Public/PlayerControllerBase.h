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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetupInput(class UInputComponent* PlayerInputComponent) noexcept;

protected:

private:
	UFUNCTION(Category = "Input Response")
	void Walk(const FInputActionValue& IAValue) noexcept;

	void RotatePlayerToFaceTheCursor(float DeltaTime) noexcept;
public:

protected:

	// Enhaced input

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAWalk;

private:
	FRotator LastRecordedRotationWithMouseInViewport;
};
