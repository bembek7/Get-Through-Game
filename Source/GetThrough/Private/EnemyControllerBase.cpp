#include "EnemyControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyControllerBase::AEnemyControllerBase()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

ETeamAttitude::Type AEnemyControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == FGenericTeamId(1))
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

FGenericTeamId AEnemyControllerBase::GetGenericTeamId() const
{
	return TeamId;
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

void AEnemyControllerBase::TargetPerceptionUpdated(AActor* const Actor, const FAIStimulus& Stimulus) const
{
	if (Stimulus.WasSuccessfullySensed())
	{
		EnemyBlackboard->SetValueAsObject(FName("Player"), Actor);
	}
}