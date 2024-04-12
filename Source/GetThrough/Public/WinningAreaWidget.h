// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinningAreaWidget.generated.h"

/**
 *
 */
class UTextBlock;

UCLASS()
class GETTHROUGH_API UWinningAreaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateTimeToWinCounter(const float TimeLeft) const;

	void SetTimeToWinVisibility(const ESlateVisibility NewVisibility);

	void SetPlayersInCounterVisibility(const ESlateVisibility NewVisibility);

protected:

private:
	UFUNCTION(BlueprintCallable)
	FText PlayersInAreaToText() const;

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeToWin;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayersInCounterText;
};
