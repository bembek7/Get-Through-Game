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
	QuitButton->OnClicked.AddUnique(QuitDelegate);

	FScriptDelegate OpenSettingsDelegate;
	OpenSettingsDelegate.BindUFunction(this, FName("OpenSettings"));
	SettingsButton->OnClicked.AddUnique(OpenSettingsDelegate);

	FScriptDelegate CreateGameDelegate;
	CreateGameDelegate.BindUFunction(GetGameInstance(), FName("StartOnlineGame"));
	CreateGameButton->OnClicked.AddUnique(CreateGameDelegate);

	FScriptDelegate FindGamesDelegate;
	FindGamesDelegate.BindUFunction(this, FName("FindGames"));
	FindGamesButton->OnClicked.AddUnique(FindGamesDelegate);

	SettingsWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList) noexcept
{
	FindGamesButton->SetIsEnabled(true);
	UE_LOG(LogTemp, Warning, TEXT("Widget updates the list"));
	FoundGames->ClearChildren();
	for (const auto& SessionFound : GamesList)
	{
		USessionFoundEntry* Entry = CreateWidget<USessionFoundEntry>(FoundGames, SessionEntryClass);
		FoundGames->AddChild(Entry);
		if(Entry)
		{
			Entry->SetSessionValues(SessionFound.Session.OwningUserName);
		}
	}
}

void UMainMenuWidget::OpenSettings() noexcept
{
	MainButtons->SetVisibility(ESlateVisibility::Collapsed);
	SettingsWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::CreateGame() noexcept
{
	UNWGameInstance* NetworkGameInstace = Cast<UNWGameInstance>(GetGameInstance());
	if (NetworkGameInstace)
	{
		NetworkGameInstace->StartOnlineGame();
	}
}

void UMainMenuWidget::FindGames() noexcept
{
	FindGamesButton->SetIsEnabled(false);
	UNWGameInstance* NetworkGameInstace = Cast<UNWGameInstance>(GetGameInstance());
	if (NetworkGameInstace)
	{
		NetworkGameInstace->FindOnlineGames();
	}
}

void UMainMenuWidget::QuitGame() const noexcept
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}