// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate = FScriptDelegate();
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate PlayAgainDelegate = FScriptDelegate();
	PlayAgainDelegate.BindUFunction(this, FName("PlayAgain"));
	PlayAgainButton->OnClicked.AddUnique(PlayAgainDelegate);
}

void UDeathWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UDeathWidget::PlayAgain() const noexcept
{
	UE_LOG(LogTemp, Warning, TEXT("Play again clicked"));
}
