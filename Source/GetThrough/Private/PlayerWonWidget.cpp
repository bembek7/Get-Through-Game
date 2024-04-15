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
}

void UPlayerWonWidget::QuitGame() const
{
	if (APlayerController* OwiningPlayer = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), OwiningPlayer, EQuitPreference::Quit, false);
	}
}