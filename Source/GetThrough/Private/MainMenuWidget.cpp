// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllerBase.h"
#include "Components/VerticalBox.h"

void UMainMenuWidget::NativeConstruct()
{
	FScriptDelegate QuitDelegate = FScriptDelegate();
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate OpenSettingsDelegate = FScriptDelegate();
	OpenSettingsDelegate.BindUFunction(this, FName("OpenSettings"));
	SettingsButton->OnClicked.AddUnique(OpenSettingsDelegate);

	FScriptDelegate PlayDelegate = FScriptDelegate();
	PlayDelegate.BindUFunction(this, FName("PlayGame"));
	PlayButton->OnClicked.AddUnique(PlayDelegate);

	SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::PlayGame() noexcept
{
	Cast<APlayerControllerBase>(GetOwningPlayer())->UnpauseGame();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OpenSettings() noexcept
{
	MainButtons->SetVisibility(ESlateVisibility::Collapsed);
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
