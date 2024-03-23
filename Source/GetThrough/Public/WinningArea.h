#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinningArea.generated.h"

class UBoxComponent;

UCLASS()
class GETTHROUGH_API AWinningArea : public AActor
{
	GENERATED_BODY()

public:
	AWinningArea() noexcept;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) const noexcept;

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) const noexcept;

public:

protected:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoxCollider;
private:
};