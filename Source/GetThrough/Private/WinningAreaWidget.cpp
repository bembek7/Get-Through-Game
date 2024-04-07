// Fill out your copyright notice in the Description page of Project Settings.

#include "WinningAreaWidget.h"
#include "Components/TextBlock.h"

void UWinningAreaWidget::UpdateCounter(const float TimeLeft) const
{
	const int TimeLeftCeiled = FMath::CeilToInt32(TimeLeft);
	const FText TextToDisplay = FText::FromString(FString::Printf(TEXT("Survive for: %d"), TimeLeftCeiled));
	if(Counter)
	{
		Counter->SetText(TextToDisplay);
	}
}