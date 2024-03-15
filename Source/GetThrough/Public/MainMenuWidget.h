// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class GETTHROUGH_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

private:
	UFUNCTION()
	void PlayGame() noexcept;

	UFUNCTION()
	void OpenSettings() noexcept;

	UFUNCTION()
	void QuitGame() const noexcept;

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUserWidget* SettingsWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* MainButtons;
};
