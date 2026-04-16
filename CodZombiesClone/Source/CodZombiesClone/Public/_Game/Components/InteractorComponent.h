// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

/*
 * Dis one is for the interactable actor. aka the one to be interacted with.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CODZOMBIESCLONE_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable)
	virtual FString GetInteractString() const;
};
