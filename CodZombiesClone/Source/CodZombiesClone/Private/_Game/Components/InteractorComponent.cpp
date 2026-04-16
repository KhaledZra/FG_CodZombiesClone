// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Components/InteractorComponent.h"

#include "CodZombiesClone.h"
#include "_Game/Data/FInteractionDataTableRow.h"


// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractorComponent::UpdateCost()
{
	if (FPointPriceDataTableRow* priceData = PriceData.GetRow<FPointPriceDataTableRow>(FString()))
	{
		Cost = priceData->Cost;
	}
}

void UInteractorComponent::OnInteract_Implementation(AActor* Interactor)
{
	UE_LOG(Khaled, Log, TEXT("Interaction activated"));
}

FString UInteractorComponent::GetInteractString_Implementation() const
{
	FString result = KeyDetails + " " + KeyName + " " + Connection + " " + InteractionDetails;
	UE_LOG(Khaled, Warning, TEXT("%s"), *result);
	if (InteractionType == Default) return result;
	
	FString interactionTypeString = InteractionType == EInteractionType::Buyable ? "Cost" : "Gain";
	FString priceString = FString::Printf(TEXT(" [%s: %d]"), *interactionTypeString, Cost);
	result += priceString;
	
	return result;
}

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Cache data from the data table
	if (FInteractionDataTableRow* data = InteractionData.GetRow<FInteractionDataTableRow>(FString()))
	{
		KeyDetails = data->KeyDetails;
		KeyName = data->KeyName;
		Connection = data->Connection;
		InteractionDetails = data->InteractionDetails;
		InteractionType = data->InteractionType;

		if (InteractionType == EInteractionType::Default) return;
		// Load price data
		PriceData = data->PriceData;
		UpdateCost();
	}
}
