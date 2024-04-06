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
#include "Perception/AISense_Hearing.h"
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

	if (IsLocalPlayerController())
	{
		InitializeCommonWidget(DeathWidget, DeathWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(PauseWidget, PauseWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(HUDWidget, HUDWidgetClass, ESlateVisibility::Collapsed);
		InitializeCommonWidget(PlayerWonWidget, PlayerWonWidgetClass, ESlateVisibility::Collapsed);
		
		ESlateVisibility MainMenuVisibility = ESlateVisibility::Collapsed;
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == "MainMenu")
		{
			MainMenuVisibility = ESlateVisibility::Visible;
		}
		InitializeCommonWidget(MainMenuWidget, MainMenuWidgetClass, MainMenuVisibility);

		WinningAreaWidget = CreateWidget<UWinningAreaWidget>(this, WinningAreaWidgetClass);
		WinningAreaWidget->AddToPlayerScreen();
		WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

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
		FocusOnWidget();
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

void APlayerControllerBase::SetupInput(UInputComponent* PlayerInputComponent) noexcept
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

void APlayerControllerBase::InitializeCommonWidget(UUserWidget*& WidgetToInitialize, const TSubclassOf<UUserWidget>& WidgetClass, ESlateVisibility InitialVisibility) noexcept
{
	WidgetToInitialize = CreateWidget<UUserWidget>(this, WidgetClass);
	WidgetToInitialize->AddToPlayerScreen();
	WidgetToInitialize->SetVisibility(InitialVisibility);
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
	return TeamId;
}

void APlayerControllerBase::PlayerDied() noexcept
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
	if (APlayerBase* PlayerPawn = Cast<APlayerBase>(GetPawn()))
	{
		PlayerPawn->TurnTorchOff();
	}
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerControllerBase::FocusOnWidget() noexcept
{
	if(HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void APlayerControllerBase::FocusOnGame() noexcept
{
	if (HUDWidget)
	{
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void APlayerControllerBase::CreateGame() noexcept
{

}

void APlayerControllerBase::PauseCalled() noexcept
{
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
	}
	FocusOnWidget();
}

void APlayerControllerBase::EnterTheWinningArea() noexcept
{
	GetWorldTimerManager().SetTimer(WinningAreaTimerHandle, this, &APlayerControllerBase::PlayerWon, TimeToWin, false);
	if (WinningAreaWidget)
	{
		WinningAreaWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerControllerBase::ExitTheWinningArea() noexcept
{
	GetWorldTimerManager().ClearTimer(WinningAreaTimerHandle);
	if (WinningAreaWidget)
	{
		WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerControllerBase::UpdateFoundGamesList(const TArray<FOnlineSessionSearchResult>& GamesList)
{
	if (UMainMenuWidget* MainMenuWidgetCasted = Cast<UMainMenuWidget>(MainMenuWidget))
	{
		MainMenuWidgetCasted->UpdateFoundGamesList(GamesList);
	}
}

float APlayerControllerBase::GetTimeLeftToWin() const noexcept
{
	const float TimeIn = GetWorldTimerManager().GetTimerElapsed(WinningAreaTimerHandle);
	return TimeToWin - TimeIn;
}

void APlayerControllerBase::Walk(const FInputActionValue& IAValue) noexcept
{
	const FVector2D MoveVector = IAValue.Get<FVector2D>();
	if (APawn* PlayerPawn = GetPawn())
	{
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorRightVector(), MoveVector.X);
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorForwardVector(), MoveVector.Y);
	}
}

void APlayerControllerBase::Look(const FInputActionValue& IAValue) noexcept
{
	const FVector2D LookVector = IAValue.Get<FVector2D>();
	GetPawn()->AddControllerYawInput(LookVector.X * MouseXSensitivity);
	GetPawn()->AddControllerPitchInput(LookVector.Y * MouseYSensitivity * -1);
}

void APlayerControllerBase::HandleShootInput() noexcept
{
	Server_Shoot();

	const APawn* PlayerPawn = GetPawn();
	if (!HasAuthority() && PlayerPawn)
	{
		PlayGunshotSound(PlayerPawn->GetActorLocation());
	}
}

void APlayerControllerBase::Server_Shoot_Implementation()
{
	
	if (const APlayerBase* PlayerPawn = Cast<APlayerBase>(GetPawn()))
	{
		FHitResult Hit;
		const FVector GunLocation = PlayerPawn->GetShootingStartLocation();
		const USceneComponent* PlayerCamera = Cast<USceneComponent>(PlayerPawn->GetComponentByClass(UCameraComponent::StaticClass()));
		const FVector CameraLocation = PlayerCamera->GetComponentLocation();
		const FVector CameraForwardVector = PlayerCamera->GetForwardVector();
		const FVector TraceEnd = CameraLocation + CameraForwardVector * 5000.f;

		FCollisionQueryParams QueryParams;

		QueryParams.AddIgnoredActor(PlayerPawn);

		GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECollisionChannel::ECC_Camera, QueryParams);

		if (AEnemyBase* EnemyHit = Cast<AEnemyBase>(Hit.GetActor()))
		{
			EnemyHit->ApplyDamage(GunDamage);
		}
		
		PlayGunshotSound(PlayerPawn->GetActorLocation());
	}
}

void APlayerControllerBase::ToggleCCTVView() noexcept
{
	if (!bInCCTVView)
	{
		if (CCTVs.IsEmpty())
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), CCTVClass, CCTVs);
		}
		if (!CCTVs.IsEmpty())
		{
			SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
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
		SetViewTargetWithBlend(GetPawn());
	}
}

void APlayerControllerBase::SwitchCCTV(const FInputActionValue& IAValue) noexcept
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

void APlayerControllerBase::SwitchCCTVForward() noexcept
{
	ViewedCCTVIndex++;
	if (ViewedCCTVIndex == CCTVs.Num())
	{
		ViewedCCTVIndex = 0;
	}
	SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
}

void APlayerControllerBase::SwitchCCTVBackward() noexcept
{
	ViewedCCTVIndex--;
	if (ViewedCCTVIndex < 0)
	{
		ViewedCCTVIndex = CCTVs.Num() - 1;
	}
	SetViewTargetWithBlend(CCTVs[ViewedCCTVIndex]);
}

void APlayerControllerBase::PlayGunshotSound(const FVector& GunLocation) const noexcept
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSound, GunLocation);
	APawn* PlayerPawn = GetPawn();
	if (PlayerPawn)
	{
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), PlayerPawn->GetActorLocation(), 1.f, PlayerPawn, GunshotSoundRange, FName("Gunshot"));
	}
}

void APlayerControllerBase::PlayerWon() noexcept
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