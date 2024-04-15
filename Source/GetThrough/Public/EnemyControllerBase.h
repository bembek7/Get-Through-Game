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
	AEnemyControllerBase();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ClearSpottedPlayerValue();

private:
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* const Actor, const FAIStimulus& Stimulus);

public:

protected:

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardData* BlackboardToUse;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviorTree;

private:
	FGenericTeamId TeamId = FGenericTeamId(1);
	UBlackboardComponent* EnemyBlackboard;
	AActor* LastSpottedActor;
	FTimerHandle PerceptionForgetHandle;
	float PerceptionForgetTime = 1.f;
};
