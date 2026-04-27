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

void UInteractionComponent::BeginInteract()
{
	if (CurrentFocusedInteractor == nullptr) return;
	
	CurrentFocusedInteractor->OnBeginInteract(GetOwner());
}

void UInteractionComponent::UpdateInteract()
{
	if (CurrentFocusedInteractor == nullptr) return;
	
	CurrentFocusedInteractor->OnUpdateInteract(GetOwner());
}

void UInteractionComponent::StopInteract()
{
	if (CurrentFocusedInteractor == nullptr) return;
	
	CurrentFocusedInteractor->OnStopInteract(GetOwner());
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
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	FHitResult OutHit;
	bool bIgnoreSelf = true;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, endLocation, TraceChannel, bTraceComplex,
	                                      ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf);

	// Lost connection
	if (!OutHit.bBlockingHit || OutHit.GetActor() == nullptr)
	{
		if (CurrentFocusedInteractor != nullptr)
		{
			StopInteract();
			CurrentFocusedInteractor = nullptr;
			
			if (InteractionUserRef) InteractionUserRef->OnUpdateInteractionUI("");
		}
		
		return;
	}
	
	bool bRequiresUIUpdate = false;
	FString newInteractionString = "";

	if (UInteractorComponent* Interactor = OutHit.GetActor()->FindComponentByClass<UInteractorComponent>())
	{
		if (CurrentFocusedInteractor != Interactor)
		{
			StopInteract();
		}
		bRequiresUIUpdate = true; // todo: testing cause revive does not update on same target
		CurrentFocusedInteractor = Interactor;
		FText inputKeyText = InteractionUserRef ? InteractionUserRef->GetInteractionKeyText() : FText::FromString("NULL");
		newInteractionString = CurrentFocusedInteractor->GetInteractString(inputKeyText);

		// UE_LOG(Khaled, Log, TEXT("Found Interactor: %s"), *OutHit.GetActor()->GetName());
	}
	else
	{
		bRequiresUIUpdate = CurrentFocusedInteractor != nullptr;
		StopInteract();
		CurrentFocusedInteractor = nullptr;
		newInteractionString = "";
	}
	
	if (bRequiresUIUpdate && InteractionUserRef)
	{
		InteractionUserRef->OnUpdateInteractionUI(newInteractionString);
	}
}
