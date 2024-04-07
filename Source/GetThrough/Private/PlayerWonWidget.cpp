// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWonWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UPlayerWonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate PlayAgainDelegate;
	PlayAgainDelegate.BindUFunction(this, FName("PlayAgain"));
	PlayAgainButton->OnClicked.AddUnique(PlayAgainDelegate);
}

void UPlayerWonWidget::QuitGame() const
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UPlayerWonWidget::PlayAgain() const
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main"), false);
}