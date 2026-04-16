// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractionUser;
class UCameraComponent;
class UInteractorComponent;
/*
 * Dis one is for the Interactor actor, aka the one that does the interacting.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CODZOMBIESCLONE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();
	
	void StartInteractionSystem(UCameraComponent* OwnerCamera);
	void StopInteractionSystem();
	void Interact();

protected:
	UPROPERTY(EditAnywhere)
	float InteractionRange;
	
	UPROPERTY(EditAnywhere)
	float InteractionTickSpeed;
	
	UPROPERTY()
	TObjectPtr<UInteractorComponent> CurrentFocusedInteractor;
	
	UPROPERTY()
	TObjectPtr<UCameraComponent> OwnerCameraRef;
	
	FTimerHandle InteractionTimerHandle;
	IInteractionUser* InteractionUserRef;
	
	void UpdateInteractionSystem();
};
