// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllerBase.h"
#include "Components/VerticalBox.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate OpenSettingsDelegate;
	OpenSettingsDelegate.BindUFunction(this, FName("OpenSettings"));
	SettingsButton->OnClicked.AddUnique(OpenSettingsDelegate);

	FScriptDelegate ContinueDelegate;
	ContinueDelegate.BindUFunction(this, FName("Continue"));
	ContinueButton->OnClicked.AddUnique(ContinueDelegate);

	if(SettingsWidget)
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPauseWidget::QuitGame() const
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPauseWidget::OpenSettings()
{
	if (MainButtons)
	{
		MainButtons->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (SettingsWidget)
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPauseWidget::Continue()
{
	if (APlayerControllerBase* const OwningPlayer = Cast<APlayerControllerBase>(GetOwningPlayer()))
	{
		OwningPlayer->FocusOnGame();
	}
	SetVisibility(ESlateVisibility::Collapsed);
}