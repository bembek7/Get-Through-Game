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

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	DeathWidget = CreateWidget<UUserWidget>(this, DeathWidgetClass);
	DeathWidget->AddToPlayerScreen();
	DeathWidget->SetVisibility(ESlateVisibility::Collapsed);

	PauseWidget = CreateWidget<UUserWidget>(this, PauseWidgetClass);
	PauseWidget->AddToPlayerScreen();
	PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

	HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	HUDWidget->AddToPlayerScreen();
	HUDWidget->SetVisibility(ESlateVisibility::Collapsed);

	WinningAreaWidget = CreateWidget<UUserWidget>(this, WinningAreaWidgetClass);
	WinningAreaWidget->AddToPlayerScreen();
	WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);

	MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	MainMenuWidget->AddToPlayerScreen();
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);

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
	PauseGame();
}

void APlayerControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
			PlayerEnhancedInputComponent->BindAction(IAShoot, ETriggerEvent::Started, this, &APlayerControllerBase::Shoot);
		}
		if (IACCTV)
		{
			PlayerEnhancedInputComponent->BindAction(IACCTV, ETriggerEvent::Started, this, &APlayerControllerBase::ToggleCCTVView);
		}
		if (IAPause)
		{
			PlayerEnhancedInputComponent->BindAction(IAPause, ETriggerEvent::Started, this, &APlayerControllerBase::PauseCalled);
		}
	}
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
	return TeamId;
}

void APlayerControllerBase::PlayerDied() noexcept
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	Cast<APlayerBase>(GetPawn())->TurnTorchOff();
	if (DeathWidget)
	{
		DeathWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerControllerBase::PauseGame() noexcept
{
	if (!IsPaused())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void APlayerControllerBase::PauseCalled() noexcept
{
	PauseWidget->SetVisibility(ESlateVisibility::Visible);
	PauseGame();
}

void APlayerControllerBase::UnpauseGame() noexcept
{
	if (IsPaused())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		HUDWidget->SetVisibility(ESlateVisibility::Visible);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void APlayerControllerBase::EnterTheWinningArea() noexcept
{
	WinningAreaWidget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerControllerBase::ExitTheWinningArea() noexcept
{
	WinningAreaWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APlayerControllerBase::Walk(const FInputActionValue& IAValue) noexcept
{
	const FVector2D MoveVector = IAValue.Get<FVector2D>();
	const float XAxis = MoveVector.X;
	const float YAxis = MoveVector.Y;
	APawn* PlayerPawn = GetPawn();
	PlayerPawn->AddMovementInput(PlayerPawn->GetActorRightVector(), XAxis);
	PlayerPawn->AddMovementInput(PlayerPawn->GetActorForwardVector(), YAxis);
}

void APlayerControllerBase::Look(const FInputActionValue& IAValue) noexcept
{
	const FVector2D LookVector = IAValue.Get<FVector2D>();
	GetPawn()->AddControllerYawInput(LookVector.X * MouseXSensitivity);
	GetPawn()->AddControllerPitchInput(LookVector.Y * MouseYSensitivity * -1);
}

void APlayerControllerBase::Shoot() noexcept
{
	const APlayerBase* PlayerPawn = Cast<APlayerBase>(GetPawn());
	FHitResult Hit;
	const FVector GunLocation = PlayerPawn->GetShootingStartLocation();
	const USceneComponent* PlayerCamera = Cast<USceneComponent>(PlayerPawn->GetComponentByClass(UCameraComponent::StaticClass()));
	const FVector CameraLocation = PlayerCamera->GetComponentLocation();
	const FVector CameraForwardVector = PlayerCamera->GetForwardVector();
	const FVector TraceEnd = CameraLocation + CameraForwardVector * 5000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerPawn);

	GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECollisionChannel::ECC_Camera, QueryParams);

	if (Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GunLocation, Hit.ImpactPoint, FColor::Yellow, false, 0.2f, 0, 2.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GunLocation, TraceEnd, FColor::Yellow, false, 0.2f, 0, 2.0f);
	}

	AEnemyBase* EnemyHit = Cast<AEnemyBase>(Hit.GetActor());
	if (EnemyHit)
	{
		EnemyHit->ApplyDamage(GunDamage);
	}

	PlayGunshotSound(GunLocation);
}

void APlayerControllerBase::ToggleCCTVView() noexcept
{
	if (GetViewTarget() != CCTV)
	{
		if (!CCTV)
		{
			CCTV = UGameplayStatics::GetActorOfClass(GetWorld(), CCTVClass);
		}
		SetViewTargetWithBlend(CCTV);
		IgnoreMoveInput = true;
		IgnoreLookInput = true;
	}
	else
	{
		IgnoreMoveInput = false;
		IgnoreLookInput = false;
		SetViewTargetWithBlend(GetPawn());
	}
}

void APlayerControllerBase::PlayGunshotSound(const FVector& GunLocation) const noexcept
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSound, GunLocation);
	APawn* PlayerPawn = GetPawn();
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), PlayerPawn->GetActorLocation(), 1.f, PlayerPawn, GunshotSoundRange, FName("Gunshot"));
}