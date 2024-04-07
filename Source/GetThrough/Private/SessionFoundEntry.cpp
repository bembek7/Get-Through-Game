// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionFoundEntry.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "NWGameInstance.h"

void USessionFoundEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate JoinGameDelegate;
	JoinGameDelegate.BindUFunction(this, FName("JoinGame"));
	if (JoinButton)
	{
		JoinButton->OnClicked.AddUnique(JoinGameDelegate);
	}
	if (MainBox)
	{
		if (Background)
		{
			Background->AddChild(MainBox);
		}
		if (SessionName)
		{
			MainBox->AddChild(SessionName);
		}
		if (JoinButton)
		{
			MainBox->AddChild(JoinButton);
		}
	}
}

void USessionFoundEntry::SetSessionValues(const FString& FoundSessionOwningUserName)
{
	if (SessionName)
	{
		SessionOwningUserName = FoundSessionOwningUserName;
		SessionName->SetText(FText::FromString(FoundSessionOwningUserName));
	}
}

void USessionFoundEntry::JoinGame() const
{
	if (UNWGameInstance* const NetworkGameInstace = Cast<UNWGameInstance>(GetGameInstance()))
	{
		NetworkGameInstace->JoinOnlineGame(SessionOwningUserName);
	}
}