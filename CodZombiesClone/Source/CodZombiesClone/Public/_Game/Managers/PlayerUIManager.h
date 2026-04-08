// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerUIManager.generated.h"

class UPlayerUI;

UCLASS()
class CODZOMBIESCLONE_API APlayerUIManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerUIManager();
	
	UFUNCTION()
	void AddPlayerUI(UPlayerUI* PlayerUI, int PlayerIndex);
	
	UFUNCTION()
	void UpdateUI(int NewScore, int PlayerIndex);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UPlayerUI>> PlayerUis;
};
