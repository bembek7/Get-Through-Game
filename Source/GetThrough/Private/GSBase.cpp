// Fill out your copyright notice in the Description page of Project Settings.

#include "GSBAse.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBase.h"
#include "PlayerBase.h"
#include "Net/UnrealNetwork.h"

void AGSBase::AnyPlayerDied() const
{
	if (AreAllPlayersDead())
	{
		NetMulticast_TriggerGameOver();
	}
}

void AGSBase::NetMulticast_TriggerGameOver_Implementation() const
{
	if (APlayerControllerBase* PlayerControllerCasted = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerControllerCasted->GameOver();
	}
}

void AGSBase::PlayerEnteredWinningArea()
{
	++NumberOfPlayersInWinningArea;
	if (NumberOfPlayersInWinningArea == GetNumberOfPlayersAlive())
	{
		NetMulticast_NotifyAllPlayersAreInWinningArea();
	}
}

void AGSBase::PlayerLeftWinningArea()
{
	--NumberOfPlayersInWinningArea;
	NetMulticast_NotifyPlayerLeftWinningArea();
}

uint32 AGSBase::GetNumberOfPlayersInWinningArea() const
{
	return NumberOfPlayersInWinningArea;
}

uint32 AGSBase::GetNumberOfPlayersAlive() const
{
	uint32 NumberOfPlayersAlive = 0;
	TArray<AActor*> AllPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), AllPlayers);
	for (auto& Player : AllPlayers)
	{
		if (APlayerBase* PlayerCasted = Cast<APlayerBase>(Player))
		{
			if (!PlayerCasted->IsDead())
			{
				++NumberOfPlayersAlive;
			}
		}
	}
	return NumberOfPlayersAlive;
}

void AGSBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGSBase, NumberOfPlayersInWinningArea);
}

void AGSBase::NetMulticast_NotifyAllPlayersAreInWinningArea_Implementation() const
{
	if (APlayerControllerBase* PlayerControllerCasted = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerControllerCasted->AllPlayersInWinningArea();
	}
}

void AGSBase::NetMulticast_NotifyPlayerLeftWinningArea_Implementation() const
{
	if (APlayerControllerBase* PlayerControllerCasted = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerControllerCasted->OtherPlayerLeftWinningArena();
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