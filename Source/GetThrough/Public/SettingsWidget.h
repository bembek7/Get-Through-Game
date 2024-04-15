#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

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

private:
	UFUNCTION()
	void ChangeResolution() const;

	UFUNCTION()
	void ChangeWindowMode() const;

	UFUNCTION()
	void ChangeFrameRate() const;

	UFUNCTION()
	void ChangeVolume() const;

	UFUNCTION()
	void Close();

	UFUNCTION()
	void SaveSettings();

	UFUNCTION()
	void LoadSettings();

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* Resolution;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* WindowMode;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UComboBoxString* FrameRate;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* Volume;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SaveButton;

	UPROPERTY(EditDefaultsOnly)
	class USoundMix* MasterSoundMix;

	UPROPERTY(EditDefaultsOnly)
	class USoundClass* MasterSoundClass;

private:

	UGameUserSettings* GameSettings;

	const TMap<FString, float> FrameRateMap =
	{
		{"30 FPS", 30.f},
		{"60 FPS", 60.f},
		{"120 FPS", 120.f},
		{"144 FPS", 144.f},
		{"Uncapped", 1000.f}
	};

	const TMap<FString, FIntPoint> ResolutionMap =
	{
		{"1280x720", FIntPoint(1280, 720)},
		{"1600x900", FIntPoint(1600, 900)},
		{"1920x1080", FIntPoint(1920, 1080)}
	};

	const TMap<FString, EWindowMode::Type> WindowModeMap =
	{
		{"Fullscreen", EWindowMode::Fullscreen},
		{"Window", EWindowMode::Windowed},
		{"Borderless Window", EWindowMode::WindowedFullscreen}
	};
};
