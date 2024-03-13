// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameSettings = UGameUserSettings::GetGameUserSettings();
	GameSettings->SetFullscreenMode(EWindowMode::Windowed);

	FScriptDelegate ChangeResolutionDelegate;
	ChangeResolutionDelegate.BindUFunction(this, FName("ChangeResolution"));
	Resolution->OnSelectionChanged.AddUnique(ChangeResolutionDelegate);

	FScriptDelegate ChangeWindowModeDelegate;
	ChangeWindowModeDelegate.BindUFunction(this, FName("ChangeWindowMode"));
	WindowMode->OnSelectionChanged.AddUnique(ChangeWindowModeDelegate);

	FScriptDelegate ChangeFrameRateDelegate;
	ChangeFrameRateDelegate.BindUFunction(this, FName("ChangeFrameRate"));
	FrameRate->OnSelectionChanged.AddUnique(ChangeFrameRateDelegate);

	FScriptDelegate ChangeVolumeDelegate;
	ChangeVolumeDelegate.BindUFunction(this, FName("ChangeVolume"));
	Volume->OnValueChanged.AddUnique(ChangeVolumeDelegate);
}

void USettingsWidget::ChangeResolution() const noexcept
{
	FString ResolutionPicked = Resolution->GetSelectedOption();
	GameSettings->SetScreenResolution(StringToResolution(ResolutionPicked));
	GameSettings->ApplySettings(true);
}

void USettingsWidget::ChangeWindowMode() const noexcept
{
	FString WindowModePicked = WindowMode->GetSelectedOption();
	GameSettings->SetFullscreenMode(StringToWindowMode(WindowModePicked));
	GameSettings->ApplySettings(true);
}

void USettingsWidget::ChangeFrameRate() const noexcept
{
	FString FrameRatePicked = FrameRate->GetSelectedOption();
	GameSettings->SetFrameRateLimit(StringToFrameRate(FrameRatePicked));
	GameSettings->ApplySettings(true);
}

void USettingsWidget::ChangeVolume() const noexcept
{
	UE_LOG(LogTemp, Warning, TEXT("New volume: %f"), Volume->GetValue());
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MasterSoundClass, Volume->GetValue());
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

float USettingsWidget::StringToFrameRate(const FString& StringFrameRate) const noexcept
{
	if (StringFrameRate == "30 FPS")
	{
		return 30;
	}
	else
	{
		if (StringFrameRate == "60 FPS")
		{
			return 60;
		}
		else
		{
			if (StringFrameRate == "120 FPS")
			{
				return 120;
			}
			else
			{
				if (StringFrameRate == "144 FPS")
				{
					return 144;
				}
			}
		}
	}
	return 1000;
}

EWindowMode::Type USettingsWidget::StringToWindowMode(const FString& StringWindowMode) const noexcept
{
	if (StringWindowMode == "Window")
	{
		return EWindowMode::Windowed;
	}
	else
	{
		if (StringWindowMode == "Borderless Window")
		{
			return EWindowMode::WindowedFullscreen;
		}
	}
	return EWindowMode::Fullscreen;
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
