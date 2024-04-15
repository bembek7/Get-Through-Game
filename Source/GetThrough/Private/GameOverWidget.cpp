// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);
}

void UGameOverWidget::QuitGame() const
{
	if (APlayerController* OwiningPlayer = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), OwiningPlayer, EQuitPreference::Quit, false);
	}
}
