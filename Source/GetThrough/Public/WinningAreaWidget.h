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
	void UpdateCounter(const float TimeLeft) const;

protected:

private:

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Counter;
};
