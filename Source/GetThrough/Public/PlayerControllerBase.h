// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GenericTeamAgentInterface.h"

#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GETTHROUGH_API APlayerControllerBase : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupInput(class UInputComponent* PlayerInputComponent) noexcept;

	virtual FGenericTeamId GetGenericTeamId() const override;

	void PlayerDied() noexcept;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(Category = "Input Response")
	void Walk(const FInputActionValue& IAValue) noexcept;

	UFUNCTION(Category = "Input Response")
	void Shoot() noexcept;

	UFUNCTION(Category = "Input Response")
	void ToggleCCTVView() noexcept;

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

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IACCTV;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* GunshotSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>DeathWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor>CCTVClass;

	UPROPERTY(EditDefaultsOnly)
	float GunDamage = 35.f;

	UPROPERTY(EditDefaultsOnly)
	float GunshotSoundRange = 2000.f;

private:
	AActor* CCTV;
	FGenericTeamId TeamId = FGenericTeamId(0);
	UUserWidget* MapWidget;
	UUserWidget* DeathWidget;
	FVector LastRecordedMouseLocation;
	FRotator LastRecordedRotationWithMouseInViewport;
};
