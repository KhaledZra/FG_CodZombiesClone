// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FpsPlayerController.generated.h"

class UPlayerUI;
class APlayerCharacter;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewControllerAtivatedDelegate, APlayerController*, Controller);

/**
 *  Simple Fps Player Controller
 *  Uses the new Enhanced Input system to manage input mapping contexts.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract, config="Game")
class CODZOMBIESCLONE_API AFpsPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	AFpsPlayerController();
	
	UFUNCTION(BlueprintPure)
	int GetPlayerIndex() const;
	
	FNewControllerAtivatedDelegate OnNewControllerActivated;

protected:
	
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(BlueprintReadOnly, Category ="Reference")
	APlayerCharacter* CharacterRef;
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	int PlayerIndex;

	/** Overrides */
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	
	void SetupPlayerInputBinds();
	void OnMoveInputTriggered(const FInputActionValue& Value);
	void OnLookInputTriggered(const FInputActionValue& Value);
	void OnJumpStarted();
	void OnJumpCompleted();
};
