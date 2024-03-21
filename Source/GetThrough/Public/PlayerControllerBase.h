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
	virtual void Tick(float DeltaTime) override;

	void SetupInput(class UInputComponent* PlayerInputComponent) noexcept;

	virtual FGenericTeamId GetGenericTeamId() const override;

	void PlayerDied() noexcept;

	void UnpauseGame() noexcept;

	UFUNCTION()
	void EnterTheWinningArea() noexcept;

	UFUNCTION()
	void ExitTheWinningArea() noexcept;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Category = "Input Response")
	void Walk(const FInputActionValue& IAValue) noexcept;

	UFUNCTION(Category = "Input Response")
	void Look(const FInputActionValue& IAValue) noexcept;

	UFUNCTION(Category = "Input Response")
	void Shoot() noexcept;

	UFUNCTION(Category = "Input Response")
	void ToggleCCTVView() noexcept;

	void PauseGame() noexcept;

	void PauseCalled() noexcept;

	void PlayGunshotSound(const FVector& GunLocation) const noexcept;

public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAWalk;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IALook;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAShoot;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IACCTV;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IAPause;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* GunshotSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>DeathWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>WinningAreaWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor>CCTVClass;

	UPROPERTY(EditDefaultsOnly)
	float GunDamage = 35.f;

	UPROPERTY(EditDefaultsOnly)
	float GunshotSoundRange = 2000.f;

	UPROPERTY(EditDefaultsOnly)
	float MouseXSensitivity = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float MouseYSensitivity = 1.f;

private:
	AActor* CCTV;
	FGenericTeamId TeamId = FGenericTeamId(0);
	UUserWidget* DeathWidget;
	UUserWidget* PauseWidget;
	UUserWidget* HUDWidget;
	UUserWidget* MainMenuWidget;
	UUserWidget* WinningAreaWidget;
	FVector LastRecordedMouseLocation;
	FRotator LastRecordedRotationWithMouseInViewport;
};
