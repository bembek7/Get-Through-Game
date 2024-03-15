// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameBase.generated.h"

class UGameUserSettings;
/**
 * 
 */
UCLASS()
class GETTHROUGH_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:
	static const int32 SaveIndex = 0;
	static const FString SaveSlotName;

	UPROPERTY()
	FIntPoint Resolution = FIntPoint(1920, 1080);
	UPROPERTY()
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::Fullscreen;
	UPROPERTY()
	float FrameRate = 60;
	UPROPERTY()
	float Volume = 1.f;
};
