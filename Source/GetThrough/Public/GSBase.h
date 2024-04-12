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

	void TriggerGameOver() const;

private:
	bool AreAllPlayersDead() const;
};
