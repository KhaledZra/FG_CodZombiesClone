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

void AFpsPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AFpsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsLocalController()) return;
	
	CharacterRef = Cast<APlayerCharacter>(InPawn);
	ensure(CharacterRef != nullptr);
	if (CharacterRef == nullptr)
	{
		UE_LOG(Khaled, Error, TEXT("Failed to get Character"));
		return;
	}
	
	SetupPlayerInputBinds();
	CharacterRef->CreatePlayerUI(this);
	
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
	
	SetupInputComponent();
}

void AFpsPlayerController::OnMoveInputTriggered(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
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
