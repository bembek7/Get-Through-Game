// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionFoundEntry.generated.h"

class UBorder;
class UHorizontalBox;
class UTextBlock;
class UButton;
/**
 *
 */
UCLASS()
class GETTHROUGH_API USessionFoundEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetSessionValues(const FString& FoundSessionOwningUserName);

private:
	UFUNCTION()
	void JoinGame() const;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* Background;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* MainBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SessionName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* JoinButton;

private:
	FString SessionOwningUserName;
};
