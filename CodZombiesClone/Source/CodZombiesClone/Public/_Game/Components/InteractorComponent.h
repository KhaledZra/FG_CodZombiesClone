// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "_Game/Data/FInteractionDataTableRow.h"
#include "InteractorComponent.generated.h"

/*
 * Dis one is for the interactable actor. aka the one to be interacted with.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class CODZOMBIESCLONE_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractorComponent();
	
	UFUNCTION(BlueprintCallable)
	void UpdateCost();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactor")
	void OnBeginInteract(AActor* Interactor);
	virtual void OnBeginInteract_Implementation(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactor")
	void OnUpdateInteract(AActor* Interactor);
	virtual void OnUpdateInteract_Implementation(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactor")
	void OnStopInteract(AActor* Interactor);
	virtual void OnStopInteract_Implementation(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interactor")
	FString GetInteractString(const FText& InputKeyText) const;
	virtual FString GetInteractString_Implementation(const FText& InputKeyText) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FDataTableRowHandle InteractionData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString KeyDetails;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString KeyName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString Connection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FString InteractionDetails;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FDataTableRowHandle PriceData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TEnumAsByte<EInteractionType> InteractionType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int Cost;
	
	virtual void BeginPlay() override;
};
