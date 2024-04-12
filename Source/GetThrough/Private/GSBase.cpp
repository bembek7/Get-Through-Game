// Fill out your copyright notice in the Description page of Project Settings.

#include "GSBAse.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBase.h"
#include "PlayerBase.h"

void AGSBase::AnyPlayerDied() const
{
	if (AreAllPlayersDead())
	{
		TriggerGameOver();
	}
}

void AGSBase::TriggerGameOver() const
{
	TArray<AActor*> AllPlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), AllPlayerControllers);
	for (auto& PlayerController : AllPlayerControllers)
	{
		if (APlayerControllerBase* PlayerControllerCasted = Cast<APlayerControllerBase>(PlayerController))
		{
			PlayerControllerCasted->GameOver();
		}
	}
}

bool AGSBase::AreAllPlayersDead() const
{
	TArray<AActor*> AllPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), AllPlayers);
	for (auto& Player : AllPlayers)
	{
		if (APlayerBase* PlayerCasted = Cast<APlayerBase>(Player))
		{
			if (!PlayerCasted->IsDead())
			{
				return false;
			}
		}
	}
	return true;
}