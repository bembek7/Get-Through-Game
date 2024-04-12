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

	APlayerController* PlayerController = GetOwningPlayer();
	
	if(PlayerController)
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
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

