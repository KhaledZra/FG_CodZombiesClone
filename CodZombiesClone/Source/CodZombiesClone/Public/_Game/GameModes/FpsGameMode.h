// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FpsGameMode.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS(abstract)
class CODZOMBIESCLONE_API AFpsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFpsGameMode();
	
	UPROPERTY(VisibleAnywhere, Category = "Global")
	TArray<TObjectPtr<AActor>> PlayerActors;
};
