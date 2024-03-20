#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"


class UComboBoxString;
class USlider;
class UGameUserSettings;
class UButton;
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
	void ChangeResolution() const noexcept;

	UFUNCTION()
	void ChangeWindowMode() const noexcept;

	UFUNCTION()
	void ChangeFrameRate() const noexcept;

	UFUNCTION()
	void ChangeVolume() const noexcept;

	UFUNCTION()
	void Close() noexcept;

	UFUNCTION()
	void SaveSettings() noexcept;

	UFUNCTION()
	void LoadSettings() noexcept;

public:

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* Resolution;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* WindowMode;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UComboBoxString* FrameRate;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* Volume;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(EditDefaultsOnly)
	USoundMix* MasterSoundMix;

	UPROPERTY(EditDefaultsOnly)
	USoundClass* MasterSoundClass;

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
