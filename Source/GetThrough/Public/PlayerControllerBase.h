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
class UWinningAreaWidget;

UCLASS()
class GETTHROUGH_API APlayerControllerBase : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	void SetupInput(class UInputComponent* PlayerInputComponent);

	virtual FGenericTeamId GetGenericTeamId() const override;

	void ControlledPlayerDied();

	void FocusOnGame();

	void SpectateCalled();

	UFUNCTION()
	void SpectateNextPlayer();

	UFUNCTION()
	void SpectatePreviousPlayer();

	UFUNCTION()
	void EnterTheWinningArea();

	UFUNCTION()
	void ExitTheWinningArea();

	void UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList);

	void GameOver();

	void AllPlayersInWinningArea();

	void OtherPlayerLeftWinningArena();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Category = "Input Response")
	void Walk(const FInputActionValue& IAValue);

	UFUNCTION(Category = "Input Response")
	void Look(const FInputActionValue& IAValue);

	UFUNCTION(Category = "Input Response")
	void HandleShootInput();

	UFUNCTION(Server, Reliable)
	void Server_Shoot() const;

	UFUNCTION(Category = "Input Response")
	void ToggleCCTVView();

	UFUNCTION(Category = "Input Response")
	void SwitchCCTV(const FInputActionValue& IAValue);

	void SwitchCCTVForward();

	void SwitchCCTVBackward();

	void FocusOnWidget();

	void PauseCalled();

	FHitResult ShootLineTrace() const;

	void PlayerWon();

	float GetTimeLeftToWin() const;

	void InitializeCommonWidget(UUserWidget*& WidgetToInitialize, const TSubclassOf<UUserWidget>& WidgetClass, const ESlateVisibility InitialVisibility); // Shorthand for initializing the widgets held by a pointer to UUserWidget

	TArray<AActor*> GetPlayersToSpectate() const;
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

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IASwitchCCTV;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>DeathWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>PlayerWonWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>SpectatorWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget>GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWinningAreaWidget>WinningAreaWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor>CCTVClass;

	UPROPERTY(EditDefaultsOnly)
	float GunDamage = 35.f;

	UPROPERTY(EditDefaultsOnly)
	float MouseXSensitivity = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float MouseYSensitivity = 1.f;

private:
	const float TimeToWin = 3.f;
	bool bInCCTVView = false;
	bool bInWinningArea = false;
	int32 ViewedCCTVIndex = 0;
	int32 SpectatedPlayerIndex = 0;
	TArray<AActor*> CCTVs;
	TArray<AActor*>PlayersToSpectate;

	FGenericTeamId TeamId = FGenericTeamId(0);
	UUserWidget* DeathWidget;
	UUserWidget* PauseWidget;
	UUserWidget* HUDWidget;
	UUserWidget* MainMenuWidget;
	UUserWidget* PlayerWonWidget;
	UUserWidget* SpectatorWidget;
	UUserWidget* GameOverWidget;
	UWinningAreaWidget* WinningAreaWidget;
	FVector LastRecordedMouseLocation;
	FRotator LastRecordedRotationWithMouseInViewport;
	FTimerHandle WinningAreaTimerHandle;
};
