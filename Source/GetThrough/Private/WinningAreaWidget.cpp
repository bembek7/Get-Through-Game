// Fill out your copyright notice in the Description page of Project Settings.

#include "WinningAreaWidget.h"
#include "Components/TextBlock.h"
#include "GSBase.h"
#include "Kismet/GameplayStatics.h"

void UWinningAreaWidget::UpdateTimeToWinCounter(const float TimeLeft) const
{
	const int TimeLeftCeiled = FMath::CeilToInt32(TimeLeft);
	const FText TextToDisplay = FText::FromString(FString::Printf(TEXT("Survive for: %d"), TimeLeftCeiled));
	if (TimeToWin)
	{
		TimeToWin->SetText(TextToDisplay);
	}
}

void UWinningAreaWidget::SetTimeToWinVisibility(const ESlateVisibility NewVisibility)
{
	TimeToWin->SetVisibility(NewVisibility);
}

void UWinningAreaWidget::SetPlayersInCounterVisibility(const ESlateVisibility NewVisibility)
{
	PlayersInCounterText->SetVisibility(NewVisibility);
}

FText UWinningAreaWidget::PlayersInAreaToText() const
{
	uint32 NumberOfPlayersAlive = 0;
	uint32 PlayersInCounter = 0;
	if (AGSBase* const GameState = Cast<AGSBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		PlayersInCounter = GameState->GetNumberOfPlayersInWinningArea();
		NumberOfPlayersAlive = GameState->GetNumberOfPlayersAlive();
	}

	const FText PlayersInText = FText::FromString(FString::Printf(TEXT("Waiting for other players %d/%d"), PlayersInCounter, NumberOfPlayersAlive));
	return PlayersInText;
}