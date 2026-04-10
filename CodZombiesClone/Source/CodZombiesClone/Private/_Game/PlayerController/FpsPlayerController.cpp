// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/_Game/PlayerController/FpsPlayerController.h"

#include "CodZombiesClone.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Public/_Game/Managers/FpsPlayerCameraManager.h"
#include "_Game/Characters/PlayerCharacter.h"

AFpsPlayerController::AFpsPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AFpsPlayerCameraManager::StaticClass();
}

int AFpsPlayerController::GetPlayerIndex() const
{
	return GetLocalPlayer()->GetLocalPlayerIndex();
}

void AFpsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	
}

void AFpsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (bPossessHasHappened) return;
	bPossessHasHappened = true;

	if (!IsLocalController()) return;
	SetupPlayerInputBinds();
	
	CharacterRef = Cast<APlayerCharacter>(InPawn);
	ensure(CharacterRef != nullptr);
	if (CharacterRef == nullptr)
	{
		UE_LOG(Khaled, Error, TEXT("Failed to get Character"));
		return;
	}
	
	int playerIndex = GetPlayerIndex();
	CharacterRef->SetupPlayer(this, GetPlayerColor(playerIndex), playerIndex);
	
	OnNewControllerActivated.Broadcast(this);
	UE_LOG(Khaled, Display, TEXT("Player Inputs Live!"));
}

void AFpsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (!IsLocalPlayerController()) return;
	
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}

void AFpsPlayerController::SetupPlayerInputBinds()
{
	UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(InputComponent);
	if (eic == nullptr)
	{
		UE_LOG(Khaled, Error, TEXT("Failed to get Enhanced Input Component"));
		return;
	}
	
	// Jumping
	eic->BindAction(JumpAction, ETriggerEvent::Started, this, &AFpsPlayerController::OnJumpStarted);
	eic->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFpsPlayerController::OnJumpCompleted);

	// Moving
	eic->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFpsPlayerController::OnMoveInputTriggered);

	// Looking/Aiming, Look uses the right stick on controllers, MouseLook uses the mouse movement
	eic->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFpsPlayerController::OnLookInputTriggered);
	eic->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFpsPlayerController::OnLookInputTriggered);
	
	// Left Fire Input
	eic->BindAction(FireAction, ETriggerEvent::Started, this, &AFpsPlayerController::OnLeftFireStarted);
	eic->BindAction(FireAction, ETriggerEvent::Completed, this, &AFpsPlayerController::OnLeftFireCompleted);
	
	SetupInputComponent();
}

void AFpsPlayerController::OnMoveInputTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector.Normalize();
	// LOG VALUE
	//UE_LOG(LogCodZombiesClone, Log, TEXT("%s"), *MovementVector.ToString());
	CharacterRef->DoMove(MovementVector.X, MovementVector.Y);
}

void AFpsPlayerController::OnLookInputTriggered(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	CharacterRef->DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AFpsPlayerController::OnJumpStarted()
{
	CharacterRef->DoJumpStart();
}

void AFpsPlayerController::OnJumpCompleted()
{
	CharacterRef->DoJumpEnd();
}

void AFpsPlayerController::OnLeftFireStarted()
{
	CharacterRef->DoLeftFireStarted();
}

void AFpsPlayerController::OnLeftFireCompleted()
{
	// rn does nothing i guess. 
}

FColor AFpsPlayerController::GetPlayerColor(int Index)
{
	// todo: Kinda Hardcoded from now, maybe get the colors from some sort of data table in future
	if (Index == 0) return FColor::Red;
	if (Index == 1) return FColor::Green;
	if (Index == 2) return FColor::Blue;
	if (Index == 3) return FColor::Yellow;
	
	return FColor::White;
}
