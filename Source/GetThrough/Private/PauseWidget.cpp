// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllerBase.h"

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
}

void UPauseWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPauseWidget::OpenSettings() const noexcept
{
	;
}

void UPauseWidget::Continue() const noexcept
{
	Cast<APlayerControllerBase>(GetOwningPlayer())->UnpauseGame();
}
