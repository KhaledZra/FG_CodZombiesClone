// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieWaveManager.generated.h"

class APlayerUIManager;
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
	TObjectPtr<APlayerUIManager> PlayerUIManagerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<TObjectPtr<AZombieSpawner>> ZombieSpawners;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TSubclassOf<AZombieCharacter> ZombieClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int ZombiesToSpawn = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float ZombieSpawnSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveStartCooldown = 5.0f;
	
	int CurrentWaveIndex = -1;
	int zombiesSpawned = 0;
	//int ZombiesAliveLimit = 0; // this is only handly if we need a limit for how many zombies can be alive at once.
	int ZombiesKilled = 0;
	
	bool bWaveActive = false;
	
	UFUNCTION()
	void OnZombieKilled();
	
	void SpawnIteration();
	void StartWaveSpawner();
	void StopWaveSpawner();
	void TriggerNextWave();
	
	FTimerHandle WaveSpawnTimerHandle;
	FTimerHandle WaveStartCooldownTimerHandle;
	
protected:
	virtual void BeginPlay() override;
};
