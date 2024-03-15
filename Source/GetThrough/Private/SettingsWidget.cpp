// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"
#include "SaveGameBase.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameSettings = UGameUserSettings::GetGameUserSettings();

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

	FScriptDelegate BackDelegate;
	BackDelegate.BindUFunction(this, FName("Close"));
	BackButton->OnClicked.AddUnique(BackDelegate);

	FScriptDelegate SaveDelegate;
	SaveDelegate.BindUFunction(this, FName("SaveSettings"));
	SaveButton->OnClicked.AddUnique(SaveDelegate);

	for (auto& ResolutionMapping : ResolutionMap)
	{
		Resolution->AddOption(ResolutionMapping.Key);
	}

	for (auto& FrameRateMapping : FrameRateMap)
	{
		FrameRate->AddOption(FrameRateMapping.Key);
	}

	for (auto& WindowModeMapping : WindowModeMap)
	{
		WindowMode->AddOption(WindowModeMapping.Key);
	}

	OnNativeVisibilityChanged.AddUFunction(this, FName("LoadSettings"));
}

void USettingsWidget::ChangeResolution() const noexcept
{
	FString ResolutionPicked = Resolution->GetSelectedOption();
	GameSettings->SetScreenResolution(*ResolutionMap.Find(ResolutionPicked));
}

void USettingsWidget::ChangeWindowMode() const noexcept
{
	FString WindowModePicked = WindowMode->GetSelectedOption();
	GameSettings->SetFullscreenMode(*WindowModeMap.Find(WindowModePicked));
}

void USettingsWidget::ChangeFrameRate() const noexcept
{
	FString FrameRatePicked = FrameRate->GetSelectedOption();
	GameSettings->SetFrameRateLimit(*FrameRateMap.Find(FrameRatePicked));
}

void USettingsWidget::ChangeVolume() const noexcept
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MasterSoundClass, Volume->GetValue());
}

void USettingsWidget::Close() noexcept
{
	for (auto Widget : GetParent()->GetAllChildren())
	{
		Widget->SetVisibility(ESlateVisibility::Visible); // temporary solution
	}
	SetVisibility(ESlateVisibility::Collapsed);
}

void USettingsWidget::SaveSettings() noexcept
{
	GameSettings->ApplySettings(true);
	USaveGameBase* Save = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(USaveGameBase::SaveSlotName, USaveGameBase::SaveIndex));
	if (!Save)
	{
		Save = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	}
	Save->Volume = Volume->GetValue();
	Save->FrameRate = GameSettings->GetFrameRateLimit();
	Save->Resolution = GameSettings->GetScreenResolution();
	Save->WindowMode = GameSettings->GetFullscreenMode();
	UGameplayStatics::SaveGameToSlot(Save, USaveGameBase::SaveSlotName, USaveGameBase::SaveIndex);

	Close();
}

void USettingsWidget::LoadSettings() noexcept
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		USaveGameBase* Save = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(USaveGameBase::SaveSlotName, USaveGameBase::SaveIndex));
		if (!Save)
		{
			Save = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
		}
		Volume->SetValue(Save->Volume);
		FrameRate->SetSelectedOption(*FrameRateMap.FindKey(Save->FrameRate));
		Resolution->SetSelectedOption(*ResolutionMap.FindKey(Save->Resolution));
		WindowMode->SetSelectedOption(*WindowModeMap.FindKey(Save->WindowMode));

		GameSettings->SetFrameRateLimit(Save->FrameRate);
		GameSettings->SetFullscreenMode(Save->WindowMode);
		GameSettings->SetScreenResolution(Save->Resolution);
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MasterSoundClass, Save->Volume);

		GameSettings->ApplySettings(true);
	}
}