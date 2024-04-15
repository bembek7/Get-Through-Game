// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void USpectatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	if(APlayerController* PlayerController = GetOwningPlayer())
	{
		FScriptDelegate SpectatePreviousPlayerDelegate;
		SpectatePreviousPlayerDelegate.BindUFunction(PlayerController, FName("SpectatePreviousPlayer"));
		BackwardButton->OnClicked.AddUnique(SpectatePreviousPlayerDelegate);

		FScriptDelegate SpectateNextPlayerDelegate;
		SpectateNextPlayerDelegate.BindUFunction(PlayerController, FName("SpectateNextPlayer"));
		ForwardButton->OnClicked.AddUnique(SpectateNextPlayerDelegate);
	}
}

void USpectatorWidget::QuitGame() const
{
	if (APlayerController* OwiningPlayer = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), OwiningPlayer, EQuitPreference::Quit, false);
	}
}

