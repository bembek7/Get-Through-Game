// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UVerticalBox;
class UScrollBox;
class USessionFoundEntry;

/**
 *
 */
UCLASS()
class GETTHROUGH_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList);

protected:

private:
	UFUNCTION()
	void OpenSettings();

	UFUNCTION()
	void CreateGame() const;

	UFUNCTION()
	void FindGames();

	UFUNCTION()
	void QuitGame() const;

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreateGameButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* FindGamesButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUserWidget* SettingsWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* MainButtons;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* FoundGames;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USessionFoundEntry>SessionEntryClass;
};
