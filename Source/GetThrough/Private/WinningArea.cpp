#include "WinningArea.h"
#include "Components/BoxComponent.h"
#include "PlayerControllerBase.h"
#include "PlayerBase.h"

AWinningArea::AWinningArea()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);
}

void AWinningArea::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate OnBeginOverlapDelegate;
	OnBeginOverlapDelegate.BindUFunction(this, FName("OnBoxBeginOverlap"));
	BoxCollider->OnComponentBeginOverlap.AddUnique(OnBeginOverlapDelegate);

	FScriptDelegate OnEndOverlapDelegate;
	OnEndOverlapDelegate.BindUFunction(this, FName("OnBoxEndOverlap"));
	BoxCollider->OnComponentEndOverlap.AddUnique(OnEndOverlapDelegate);
}

void AWinningArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) const
{
	if (APlayerBase* const PlayerPawn = Cast<APlayerBase>(OtherActor))
	{
		if (APlayerControllerBase* const PlayerController = Cast<APlayerControllerBase>(PlayerPawn->GetController()))
		{
			PlayerController->EnterTheWinningArea();
		}
	}
}

void AWinningArea::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) const
{
	if (APlayerBase* const PlayerPawn = Cast<APlayerBase>(OtherActor))
	{
		if (APlayerControllerBase* const PlayerController = Cast<APlayerControllerBase>(PlayerPawn->GetController()))
		{
			PlayerController->ExitTheWinningArea();
		}
	}
}