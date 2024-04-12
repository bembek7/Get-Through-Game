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
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UDeathWidget::Spectate()
{
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetOwningPlayer());

	if (PlayerController)
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
		GetOwningPlayer()->GetWorldTimerManager().SetTimer(EnableSpectateButtonTimer, [this]() { SpectateButton->SetIsEnabled(true); }, 3.f, false);
	}
}
