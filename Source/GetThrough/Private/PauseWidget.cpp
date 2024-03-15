// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllerBase.h"
#include "Components/VerticalBox.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate = FScriptDelegate();
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate OpenSettingsDelegate = FScriptDelegate();
	OpenSettingsDelegate.BindUFunction(this, FName("OpenSettings"));
	SettingsButton->OnClicked.AddUnique(OpenSettingsDelegate);

	FScriptDelegate ContinueDelegate = FScriptDelegate();
	ContinueDelegate.BindUFunction(this, FName("Continue"));
	ContinueButton->OnClicked.AddUnique(ContinueDelegate);

	SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UPauseWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPauseWidget::OpenSettings() noexcept
{
	MainButtons->SetVisibility(ESlateVisibility::Collapsed);
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
}

void UPauseWidget::Continue() const noexcept
{
	Cast<APlayerControllerBase>(GetOwningPlayer())->UnpauseGame();
}
