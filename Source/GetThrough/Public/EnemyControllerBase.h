// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"


#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GETTHROUGH_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyControllerBase() noexcept;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) const noexcept;

public:

protected:

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardData* BlackboardToUse;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviorTree;

private:

	UBlackboardComponent* EnemyBlackboard;
};
