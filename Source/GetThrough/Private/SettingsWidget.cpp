// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameSettings = UGameUserSettings::GetGameUserSettings();
	GameSettings->SetFullscreenMode(EWindowMode::Windowed);
	FScriptDelegate ChangeResolutionDelegate;
	ChangeResolutionDelegate.BindUFunction(this, FName("ChangeResolution"));
	Resolution->OnSelectionChanged.AddUnique(ChangeResolutionDelegate);
}

void USettingsWidget::ChangeResolution() const noexcept
{
	FString ResolutionPicked = Resolution->GetSelectedOption();
	GameSettings->SetScreenResolution(StringToResolution(ResolutionPicked));
	GameSettings->ApplySettings(true);
}


// I don't like this solution but, I think the only thing you could do to avoid it would be a button for every resolution.
FIntPoint USettingsWidget::StringToResolution(const FString& StringResolution) const noexcept
{
	if (StringResolution == "1280x720")
	{
		return FIntPoint(1280, 720);
	}
	else
	{
		if (StringResolution == "1600x900")
		{
			return FIntPoint(1600, 900);
		}
	}
	return FIntPoint(1920, 1080);
}
