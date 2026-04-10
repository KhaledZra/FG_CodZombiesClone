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
	TArray<TObjectPtr<UInputMappingContext>> DefaultMappingContexts;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(BlueprintReadOnly, Category ="Reference")
	TObjectPtr<APlayerCharacter> CharacterRef;
	
	UPROPERTY(BlueprintReadOnly, Category="Player")
	int PlayerIndex;

	bool bPossessHasHappened = false;

	/** Overrides */
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	
	void SetupPlayerInputBinds();
	void OnMoveInputTriggered(const FInputActionValue& Value);
	void OnLookInputTriggered(const FInputActionValue& Value);
	void OnJumpStarted();
	void OnJumpCompleted();
	void OnLeftFireStarted();
	void OnLeftFireCompleted();
	FColor GetPlayerColor(int Index);
};
