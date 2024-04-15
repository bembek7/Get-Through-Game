// Fill out your copyright notice in the Description page of Project Settings.

#include "NWGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllerBase.h"

UNWGameInstance::UNWGameInstance(const FObjectInitializer& ObjectInitializer)
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UNWGameInstance::OnDestroySessionComplete);
}

void UNWGameInstance::StartOnlineGame()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), FName("Session Name"), true, true, 4);
}

void UNWGameInstance::FindOnlineGames()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), true, true);
}

void UNWGameInstance::JoinOnlineGame(const FString& ChosenSessionOwningUserName)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	for (const auto& SearchResult : SessionSearch->SearchResults)
	{
		if (SearchResult.Session.OwningUserId != Player->GetPreferredUniqueNetId() && SearchResult.Session.OwningUserName == ChosenSessionOwningUserName)
		{
			JoinFoundSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), FName("Session Name"), SearchResult);
			break;
		}
	}
}

void UNWGameInstance::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(FName("Session Name"));
		}
	}
}

bool UNWGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, uint32 MaxNumPlayers)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("Main"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}

	return false;
}

void UNWGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UNWGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "/Game/Levels/Main", true, "listen");
	}
}

void UNWGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void UNWGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			APlayerControllerBase* const PlayerController = Cast<APlayerControllerBase>(GetFirstLocalPlayerController());
			if (PlayerController)
			{
				PlayerController->UpdateFoundGamesList(SessionSearch->SearchResults);
			}
		}
	}
}

bool UNWGameInstance::JoinFoundSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	bool bSuccessful = false;
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UNWGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UNWGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", true);
			}
		}
	}
}