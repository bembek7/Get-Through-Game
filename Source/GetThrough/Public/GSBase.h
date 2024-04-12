// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GSBAse.generated.h"

/**
 *
 */
UCLASS()
class GETTHROUGH_API AGSBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	void AnyPlayerDied() const;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_TriggerGameOver() const;

	void PlayerEnteredWinningArea();

	void PlayerLeftWinningArea();

	uint32 GetNumberOfPlayersInWinningArea() const;

	uint32 GetNumberOfPlayersAlive() const;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_NotifyAllPlayersAreInWinningArea() const;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_NotifyPlayerLeftWinningArea() const;

private:
	bool AreAllPlayersDead() const;

	UPROPERTY(Replicated)
	uint32 NumberOfPlayersInWinningArea = 0;
};
