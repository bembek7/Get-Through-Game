// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWonWidget.generated.h"

/**
 * Right now it looks same as a death widget class, but in the future it may need some additional functionalities
 */
UCLASS()
class GETTHROUGH_API UPlayerWonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:

private:
	UFUNCTION()
	void QuitGame() const;

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton;
};
