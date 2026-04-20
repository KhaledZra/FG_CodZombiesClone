// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieWaveManager.generated.h"

class AZombieCharacter;
class AZombieSpawner;

UCLASS()
class CODZOMBIESCLONE_API AZombieWaveManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieWaveManager();
	
	int GetCurrentWave() const { return CurrentWaveIndex + 1; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<TObjectPtr<AZombieSpawner>> ZombieSpawners;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<AZombieCharacter> ZombieClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int ZombiesToSpawn = 0;
	
	int CurrentWaveIndex = 0;
	int zombiesSpawned = 0;
	int ZombiesAlive = 0;
	int ZombiesKilled = 0;
	
	bool bWaveActive = false;
	
	UFUNCTION()
	void OnZombieKilled();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
