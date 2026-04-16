// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Components/InteractorComponent.h"

#include "CodZombiesClone.h"


// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractorComponent::OnInteract_Implementation(AActor* Interactor)
{
	UE_LOG(Khaled, Log, TEXT("Interaction activated"));
}

FString UInteractorComponent::GetInteractString_Implementation() const
{
	return "E - NULL";
}