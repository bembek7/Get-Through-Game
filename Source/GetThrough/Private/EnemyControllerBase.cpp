// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyControllerBase.h"

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

void AEnemyControllerBase::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) const noexcept
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
}
