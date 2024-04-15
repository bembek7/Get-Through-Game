// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

/**
 *
 */
UCLASS()
class GETTHROUGH_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

private:
	UFUNCTION()
	void QuitGame() const;

	UFUNCTION()
	void Spectate();

	UFUNCTION()
	void VisibilityChanged();

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SpectateButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;

	FTimerHandle EnableSpectateButtonHandle;

private:
	float SpectationAllowanceDelay = 3.f;
};
