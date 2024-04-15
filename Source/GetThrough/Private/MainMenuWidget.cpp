// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllerBase.h"
#include "Components/VerticalBox.h"
#include "NWGameInstance.h"
#include "Components/ScrollBox.h"
#include "SessionFoundEntry.h"
#include "OnlineSessionSettings.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate QuitDelegate;
	QuitDelegate.BindUFunction(this, FName("QuitGame"));
	if (QuitButton)
	{
		QuitButton->OnClicked.AddUnique(QuitDelegate);
	}

	FScriptDelegate OpenSettingsDelegate;
	OpenSettingsDelegate.BindUFunction(this, FName("OpenSettings"));
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddUnique(OpenSettingsDelegate);
	}

	FScriptDelegate CreateGameDelegate;
	CreateGameDelegate.BindUFunction(GetGameInstance(), FName("StartOnlineGame"));
	if (CreateGameButton)
	{
		CreateGameButton->OnClicked.AddUnique(CreateGameDelegate);
	}

	FScriptDelegate FindGamesDelegate;
	FindGamesDelegate.BindUFunction(this, FName("FindGames"));
	if (FindGamesButton)
	{
		FindGamesButton->OnClicked.AddUnique(FindGamesDelegate);
	}

	if(SettingsWidget)
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenuWidget::UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList)
{
	if (FindGamesButton)
	{
		FindGamesButton->SetIsEnabled(true);
	}
	if (FoundGames)
	{
		FoundGames->ClearChildren();
		for (const auto& SessionFound : GamesList)
		{
			USessionFoundEntry* Entry = CreateWidget<USessionFoundEntry>(FoundGames, SessionEntryClass);
			FoundGames->AddChild(Entry);
			Entry->SetSessionValues(SessionFound.Session.OwningUserName);
		}
	}
}

void UMainMenuWidget::OpenSettings()
{
	if (MainButtons)
	{
		MainButtons->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (SettingsWidget)
	{
		SettingsWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::CreateGame() const
{
	if (UNWGameInstance* const NetworkGameInstace = Cast<UNWGameInstance>(GetGameInstance()))
	{
		NetworkGameInstace->StartOnlineGame();
	}
}

void UMainMenuWidget::FindGames()
{
	if (FindGamesButton)
	{
		FindGamesButton->SetIsEnabled(false);
	}
	if (UNWGameInstance* const NetworkGameInstace = Cast<UNWGameInstance>(GetGameInstance()))
	{
		NetworkGameInstace->FindOnlineGames();
	}
}

void UMainMenuWidget::QuitGame() const
{
	if (APlayerController* OwiningPlayer = GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), OwiningPlayer, EQuitPreference::Quit, false);
	}
}