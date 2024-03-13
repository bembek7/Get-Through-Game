// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"


class UComboBoxString;
class USlider;
class UGameUserSettings;
/**
 * 
 */
UCLASS()
class GETTHROUGH_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* Resolution;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* WindowMode;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* FrameRate;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* Volume;

private:
	UFUNCTION()
	void ChangeResolution() const noexcept;

	FIntPoint StringToResolution(const FString& StringResolution) const noexcept;

	UGameUserSettings* GameSettings;
};
