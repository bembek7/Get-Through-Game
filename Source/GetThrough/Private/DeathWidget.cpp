// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBase.h"

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate SpectateDelegate;
	SpectateDelegate.BindUFunction(this, FName("Spectate"));
	SpectateButton->OnClicked.AddUnique(SpectateDelegate);

	FScriptDelegate OnVisibilityChangedDelegate;
	OnVisibilityChangedDelegate.BindUFunction(this, FName("VisibilityChanged"));
	OnVisibilityChanged.AddUnique(OnVisibilityChangedDelegate);
}

void UDeathWidget::QuitGame() const
{
	if(APlayerController* OwiningPlayer = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), OwiningPlayer, EQuitPreference::Quit, false);
	}
}

void UDeathWidget::Spectate()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetOwningPlayer()))
	{
		PlayerController->SpectateCalled();
	}
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDeathWidget::VisibilityChanged()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SpectateButton->SetIsEnabled(false);
		if (APlayerController* OwiningPlayer = GetOwningPlayer())
		{
			GetOwningPlayer()->GetWorldTimerManager().SetTimer(EnableSpectateButtonHandle, [this]() { SpectateButton->SetIsEnabled(true); }, SpectationAllowanceDelay, false);

		}
	}
}