// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionFoundEntry.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void USessionFoundEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate JoinGameDelegate;
	JoinGameDelegate.BindUFunction(this, FName("JoinGame"));
	JoinButton->OnClicked.AddUnique(JoinGameDelegate);

	Background->AddChild(MainBox);
	MainBox->AddChild(SessionName);
	MainBox->AddChild(SessionNrOfPlayers);
	MainBox->AddChild(SessionPing);
	MainBox->AddChild(JoinButton);

}

void USessionFoundEntry::SetSessionValues(const FString& FoundSessionName) noexcept
{
	if (SessionName)
	{
		SessionName->SetText(FText::FromString(FoundSessionName.Left(20)));
		UE_LOG(LogTemp, Warning, TEXT("session name found"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("session name not found"));
	}
}

void USessionFoundEntry::JoinGame() noexcept
{
}
