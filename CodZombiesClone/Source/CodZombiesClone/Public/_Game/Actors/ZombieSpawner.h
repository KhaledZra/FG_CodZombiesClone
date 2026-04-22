// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

struct FZombieStats;
class AZombieCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZombieKilledDelegate);

UCLASS()
class CODZOMBIESCLONE_API AZombieSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieSpawner();
	
	FZombieKilledDelegate OnZombieKilled;
	
	bool TrySpawnZombie(const TSubclassOf<AZombieCharacter>& ZombieClass, const FZombieStats& Stats);
	// on zombie death?
	
protected:
	UFUNCTION()
	void NotifyZombieKilled(AActor* ZombieActor);
	
};
