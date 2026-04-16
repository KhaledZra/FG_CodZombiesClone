// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Components/InteractionComponent.h"

#include "CodZombiesClone.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "_Game/Components/InteractorComponent.h"
#include "_Game/Interfaces/InteractionUser.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InteractionRange = 1000.0f;
	InteractionTickSpeed = 0.5f;
}

void UInteractionComponent::StartInteractionSystem(UCameraComponent* OwnerCamera)
{
	OwnerCameraRef = OwnerCamera;

	InteractionUserRef = Cast<IInteractionUser>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(InteractionTimerHandle,
	                                       this,
	                                       &UInteractionComponent::UpdateInteractionSystem,
	                                       InteractionTickSpeed,
	                                       true);
}

void UInteractionComponent::StopInteractionSystem()
{
	GetWorld()->GetTimerManager().ClearTimer(InteractionTimerHandle);
}

void UInteractionComponent::Interact()
{
	if (CurrentFocusedInteractor == nullptr) return;
}

void UInteractionComponent::UpdateInteractionSystem()
{
	FVector startLocation = OwnerCameraRef->GetComponentLocation();
	FVector endLocation = OwnerCameraRef->GetComponentLocation() + (OwnerCameraRef->GetForwardVector() *
		InteractionRange);

	// Do the linetrace stuff
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	bool bTraceComplex = true;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	FHitResult OutHit;
	bool bIgnoreSelf = true;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, endLocation, TraceChannel, bTraceComplex,
	                                      ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf);

	if (!OutHit.bBlockingHit || OutHit.GetActor() == nullptr)
	{
		if (CurrentFocusedInteractor != nullptr)
		{
			CurrentFocusedInteractor = nullptr;
			
			if (InteractionUserRef) InteractionUserRef->OnUpdateInteractionUI("");
		}
		
		return;
	}
	
	bool bRequiresUIUpdate = false;
	FString newInteractionString = "";

	if (UInteractorComponent* Interactor = OutHit.GetActor()->FindComponentByClass<UInteractorComponent>())
	{
		bRequiresUIUpdate = Interactor != CurrentFocusedInteractor;
		CurrentFocusedInteractor = Interactor;
		newInteractionString = CurrentFocusedInteractor->GetInteractString();

		UE_LOG(Khaled, Log, TEXT("Found Interactor: %s"), *OutHit.GetActor()->GetName());
	}
	else
	{
		bRequiresUIUpdate = CurrentFocusedInteractor != nullptr;
		CurrentFocusedInteractor = nullptr;
		newInteractionString = "";
	}
	
	if (bRequiresUIUpdate && InteractionUserRef)
	{
		InteractionUserRef->OnUpdateInteractionUI(newInteractionString);
	}
}
