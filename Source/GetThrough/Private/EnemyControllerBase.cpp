// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyControllerBase::AEnemyControllerBase() noexcept
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	
}

void AEnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardToUse)
	{
		UseBlackboard(BlackboardToUse, EnemyBlackboard);
	}

	if (DefaultBehaviorTree)
	{
		RunBehaviorTree(DefaultBehaviorTree);
	}

	FScriptDelegate TargetPereceptionUpdatedDelegate;
	TargetPereceptionUpdatedDelegate.BindUFunction(this, FName("TargetPerceptionUpdated"));
	AIPerception->OnTargetPerceptionUpdated.AddUnique(TargetPereceptionUpdatedDelegate);
}

void AEnemyControllerBase::TargetPerceptionUpdated(AActor* Actor, const FAIStimulus& Stimulus) const noexcept
{
	if (Stimulus.WasSuccessfullySensed())
	{
		EnemyBlackboard->SetValueAsObject(FName("Player"), Actor);
	}
}
