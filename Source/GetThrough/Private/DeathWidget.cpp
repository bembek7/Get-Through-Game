// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate PlayAgainDelegate;
	PlayAgainDelegate.BindUFunction(this, FName("PlayAgain"));
	PlayAgainButton->OnClicked.AddUnique(PlayAgainDelegate);
}

void UDeathWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UDeathWidget::PlayAgain() const noexcept
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main"), false);
}