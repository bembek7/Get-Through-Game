// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectatorWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class GETTHROUGH_API USpectatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

private:
	UFUNCTION()
	void QuitGame() const;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ForwardButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackwardButton;

};
