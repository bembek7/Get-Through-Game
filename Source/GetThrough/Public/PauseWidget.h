// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 *
 */
UCLASS()
class GETTHROUGH_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

private:
	UFUNCTION()
	void QuitGame() const;

	UFUNCTION()
	void OpenSettings();

	UFUNCTION()
	void Continue();

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ContinueButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUserWidget* SettingsWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* MainButtons;
};
