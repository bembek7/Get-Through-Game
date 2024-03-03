// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

class UButton;

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
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayAgainButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

private:
	UFUNCTION()
	void QuitGame() const noexcept;

	UFUNCTION()
	void PlayAgain() const noexcept;
};
