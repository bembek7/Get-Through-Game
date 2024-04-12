// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBase.h"
#include "Camera/CameraComponent.h"
#include "SaveGameBase.h"
#include "WinningAreaWidget.h"
#include "Net/UnrealNetwork.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "MainMenuWidget.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMappingContext.IsNull())
			{
				InputSystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 2);
			}
		}
	}

	if (IsLocalPlayerController())
	{
		InitializeCommonWidget(DeathWidget, DeathWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(PauseWidget, PauseWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(HUDWidget, HUDWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(PlayerWonWidget, PlayerWonWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(SpectatorWidget, SpectatorWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(GameOverWidget, GameOverWidgetClass, ESlateVisibility::Collapsed);

		WinningAreaWidget = CreateWidget<UWinningAreaWidget>(this, WinningAreaWidgetClass);
		WinningAreaWidget->AddToPlayerScreen();
		WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);

		ESlateVisibility MainMenuVisibility = ESlateVisibility::Collapsed;
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MainMenu")
		{
			MainMenuVisibility = ESlateVisibility::Visible;
			FocusOnWidget();
		}
		else
		{
			FocusOnGame();
		}

		InitializeCommonWidget(MainMenuWidget, MainMenuWidgetClass, MainMenuVisibility);
	}
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WinningAreaWidget && WinningAreaWidget->IsVisible())
	{
		WinningAreaWidget->UpdateCounter(GetTimeLeftToWin());
	}
}

void APlayerControllerBase::SetupInput(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Binding inputs
		if (IAWalk)
		{
			PlayerEnhancedInputComponent->BindAction(IAWalk, ETriggerEvent::Triggered, this, &APlayerControllerBase::Walk);
		}
		if (IALook)
		{
			PlayerEnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerControllerBase::Look);
		}
		if (IAShoot)
		{
			PlayerEnhancedInputComponent->BindAction(IAShoot, ETriggerEvent::Started, this, &APlayerControllerBase::HandleShootInput);
		}
		if (IACCTV)
		{
			PlayerEnhancedInputComponent->BindAction(IACCTV, ETriggerEvent::Started, this, &APlayerControllerBase::ToggleCCTVView);
		}
		if (IAPause)
		{
			PlayerEnhancedInputComponent->BindAction(IAPause, ETriggerEvent::Started, this, &APlayerControllerBase::PauseCalled);
		}
		if (IASwitchCCTV)
		{
			PlayerEnhancedInputComponent->BindAction(IASwitchCCTV, ETriggerEvent::Started, this, &APlayerControllerBase::SwitchCCTV);
		}
	}
}

void APlayerControllerBase::InitializeCommonWidget(UUserWidget*& WidgetToInitialize, const TSubclassOf<UUserWidget>& WidgetClass, const ESlateVisibility InitialVisibility)
{
	WidgetToInitialize = CreateWidget<UUserWidget>(this, WidgetClass);
	WidgetToInitialize->AddToPlayerScreen();
	WidgetToInitialize->SetVisibility(InitialVisibility);
}

TArray<AActor*> APlayerControllerBase::GetPlayersToSpectate() const
{
	TArray<AActor*>AllPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), AllPlayers);
	for (const auto& PlayerFound : AllPlayers)
	{
		if (APlayerBase* const PlayerFoundCasted = Cast<APlayerBase>(PlayerFound))
		{
			if (PlayerFoundCasted->IsDead())
			{
				AllPlayers.Remove(PlayerFoundCasted);
			}
		}
	}

	return AllPlayers;
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
	return TeamId;
}

void APlayerControllerBase::ControlledPlayerDied()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	if (HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (APlayerBase* const PlayerPawn = Cast<APlayerBase>(GetPawn()))
	{
		PlayerPawn->TurnTorchOff();
	}
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}
	// TODO check if everybody dead
}

void APlayerControllerBase::FocusOnWidget()
{
	if (HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void APlayerControllerBase::FocusOnGame()
{
	if (HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void APlayerControllerBase::SpectateCalled()
{
	PlayersToSpectate = GetPlayersToSpectate();
	SpectatedPlayerIndex = 0;
	if (PlayersToSpectate.Num() > 0)
	{
		if (PlayersToSpectate[SpectatedPlayerIndex])
		{
			SetViewTargetWithBlend(PlayersToSpectate[SpectatedPlayerIndex]);
			SpectatorWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APlayerControllerBase::SpectateNextPlayer()
{
	PlayersToSpectate = GetPlayersToSpectate();
	if (PlayersToSpectate.Num() > 0)
	{
		++SpectatedPlayerIndex;
		if (SpectatedPlayerIndex >= PlayersToSpectate.Num())
		{
			SpectatedPlayerIndex = 0;
		}
		if (PlayersToSpectate[SpectatedPlayerIndex])
		{
			SetViewTargetWithBlend(PlayersToSpectate[SpectatedPlayerIndex]);
		}
	}
}

void APlayerControllerBase::SpectatePreviousPlayer()
{
	PlayersToSpectate = GetPlayersToSpectate();
	if (PlayersToSpectate.Num() > 0)
	{
		--SpectatedPlayerIndex;
		if (SpectatedPlayerIndex < 0)
		{
			SpectatedPlayerIndex = PlayersToSpectate.Num() - 1;
		}
		if (PlayersToSpectate[SpectatedPlayerIndex])
		{
			SetViewTargetWithBlend(PlayersToSpectate[SpectatedPlayerIndex]);
		}
	}
}

void APlayerControllerBase::PauseCalled()
{
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
	FocusOnWidget();
}

void APlayerControllerBase::EnterTheWinningArea()
{
	GetWorldTimerManager().SetTimer(WinningAreaTimerHandle, this, &APlayerControllerBase::PlayerWon, TimeToWin, false);
	if (WinningAreaWidget)
	{
		WinningAreaWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerControllerBase::ExitTheWinningArea()
{
	GetWorldTimerManager().ClearTimer(WinningAreaTimerHandle);
	if (WinningAreaWidget)
	{
		WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerControllerBase::UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList)
{
	if (UMainMenuWidget* const MainMenuWidgetCasted = Cast<UMainMenuWidget>(MainMenuWidget))
	{
		MainMenuWidgetCasted->UpdateFoundGamesList(GamesList);
	}
}

void APlayerControllerBase::GameOver()
{
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (SpectatorWidget)
	{
		SpectatorWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (GameOverWidget)
	{
		GameOverWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

float APlayerControllerBase::GetTimeLeftToWin() const
{
	const float TimeIn = GetWorldTimerManager().GetTimerElapsed(WinningAreaTimerHandle);
	return TimeToWin - TimeIn;
}

void APlayerControllerBase::Walk(const FInputActionValue& IAValue)
{
	const FVector2D MoveVector = IAValue.Get<FVector2D>();
	if (APawn* const PlayerPawn = GetPawn())
	{
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorRightVector(), MoveVector.X);
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorForwardVector(), MoveVector.Y);
	}
}

void APlayerControllerBase::Look(const FInputActionValue& IAValue)
{
	const FVector2D LookVector = IAValue.Get<FVector2D>();
	if (APawn* const PlayerPawn = GetPawn())
	{
		PlayerPawn->AddControllerYawInput(LookVector.X * MouseXSensitivity);
		PlayerPawn->AddControllerPitchInput(LookVector.Y * MouseYSensitivity * -1);
	}
}

FHitResult APlayerControllerBase::ShootLineTrace() const
{
	FHitResult Hit;
	if (APlayerBase* const PlayerPawn = Cast<APlayerBase>(GetPawn()))
	{
		const FVector TraceStart = PlayerPawn->GetShootingTraceStartPointLocation();
		const FVector ShootingDirection = PlayerPawn->GetShootingDirection();
		const FVector TraceEnd = TraceStart + ShootingDirection * 5000.f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(PlayerPawn);

		GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Camera, QueryParams);
	}

	return Hit;
}

void APlayerControllerBase::HandleShootInput()
{
	Server_Shoot();
}

void APlayerControllerBase::Server_Shoot_Implementation() const
{
	const FHitResult Hit = ShootLineTrace();
	if (AEnemyBase* const EnemyHit = Cast<AEnemyBase>(Hit.GetActor()))
	{
		EnemyHit->ApplyDamage(GunDamage);
	}

	if (APlayerBase* const PlayerPawn = Cast<APlayerBase>(GetPawn()))
	{
		const FVector GunLocation = PlayerPawn->GetShotBeamStartLocation();
		const FVector TraceEnd = (Hit.bBlockingHit) ? Hit.ImpactPoint : Hit.TraceEnd;
		PlayerPawn->NetMulticast_DrawBulletTrace(GunLocation, TraceEnd - GunLocation);
		PlayerPawn->NetMulticast_PlayGunshotSound();
	}
}

void APlayerControllerBase::ToggleCCTVView()
{
	if (!bInCCTVView)
	{
		if (CCTVs.IsEmpty())
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), CCTVClass, CCTVs);
		}
		if (!CCTVs.IsEmpty())
		{
			if (CCTVs[ViewedCCTVIndex])
			{
				SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
			}
			IgnoreMoveInput = true;
			IgnoreLookInput = true;
			bInCCTVView = true;
		}
	}
	else
	{
		bInCCTVView = false;
		IgnoreMoveInput = false;
		IgnoreLookInput = false;
		if (APawn* const PlayerPawn = GetPawn())
		{
			SetViewTargetWithBlend(PlayerPawn);
		}
	}
}

void APlayerControllerBase::SwitchCCTV(const FInputActionValue& IAValue)
{
	if (bInCCTVView)
	{
		const float bForward = IAValue.Get<float>() > 0;
		if (bForward)
		{
			SwitchCCTVForward();
		}
		else
		{
			SwitchCCTVBackward();
		}
	}
}

void APlayerControllerBase::SwitchCCTVForward()
{
	++ViewedCCTVIndex;
	if (ViewedCCTVIndex == CCTVs.Num())
	{
		ViewedCCTVIndex = 0;
	}
	if (CCTVs[ViewedCCTVIndex])
	{
		SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
	}
}

void APlayerControllerBase::SwitchCCTVBackward()
{
	--ViewedCCTVIndex;
	if (ViewedCCTVIndex < 0)
	{
		ViewedCCTVIndex = CCTVs.Num() - 1;
	}
	if (CCTVs[ViewedCCTVIndex])
	{
		SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
	}
}

void APlayerControllerBase::PlayerWon()
{
	if (WinningAreaWidget)
	{
		WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (PlayerWonWidget)
	{
		PlayerWonWidget->SetVisibility(ESlateVisibility::Visible);
	}
	FocusOnWidget();
}