// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
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
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

private:
	UFUNCTION()
	void QuitGame() const noexcept;

	UFUNCTION()
	void OpenSettings() const noexcept;

	UFUNCTION()
	void Continue() const noexcept;
};
