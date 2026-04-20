// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Managers/ZombieWaveManager.h"

#include "CodZombiesClone.h"
#include "EngineUtils.h"
#include "_Game/Actors/ZombieSpawner.h"
#include "_Game/Managers/PlayerUIManager.h"


// Sets default values
AZombieWaveManager::AZombieWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AZombieWaveManager::OnZombieKilled()
{
	UE_LOG(Khaled, Log, TEXT("AZombieWaveManager::OnZombieKilled - Zombie Killed!"));
	ZombiesKilled++;

	if (ZombiesKilled >= ZombiesToSpawn)
	{
		UE_LOG(Khaled, Warning, TEXT("AZombieWaveManager::OnZombieKilled - Wave Cleared!"));
		TriggerNextWave();
	}
}

void AZombieWaveManager::BeginPlay()
{
	Super::BeginPlay();
	
	// both of these work.
	//for (TActorIterator<AZombieSpawner> Itr(GetWorld()); Itr; ++Itr)
	for (AZombieSpawner* spawner : TActorRange<AZombieSpawner>(GetWorld()))
	{
		spawner->OnZombieKilled.AddDynamic(this, &AZombieWaveManager::OnZombieKilled);
		ZombieSpawners.Add(*spawner);
	}
	
	// Get Player UI Manager ref
	if (const TActorIterator<APlayerUIManager> Itr(GetWorld()); Itr)
	{
		PlayerUIManagerRef = *Itr;
	}

	if (ZombieSpawners.IsEmpty())
	{
		UE_LOG(Khaled, Error, TEXT("AZombieWaveManager::BeginPlay - No Zombie Spawners found in the world!"));
	}
	else
	{
		TriggerNextWave();
	}
}

void AZombieWaveManager::SpawnIteration()
{
	if (!bWaveActive) return;

	AZombieSpawner* rndSpawner = ZombieSpawners[FMath::RandRange(0, ZombieSpawners.Num() - 1)];
	bool bIsSpawned = rndSpawner->TrySpawnZombie(ZombieClass);

	if (!bIsSpawned) return;

	zombiesSpawned++;

	if (zombiesSpawned >= ZombiesToSpawn)
	{
		StopWaveSpawner();
	}
}

void AZombieWaveManager::StartWaveSpawner()
{
	UE_LOG(Khaled, Warning, TEXT("AZombieWaveManager::BeginPlay - Wave %i Started!"), GetCurrentWave());
	bWaveActive = true;
	GetWorld()->GetTimerManager().SetTimer(WaveSpawnTimerHandle, this, &AZombieWaveManager::SpawnIteration,
	                                       ZombieSpawnSpeed, true);
}

void AZombieWaveManager::StopWaveSpawner()
{
	bWaveActive = false;
	GetWorld()->GetTimerManager().ClearTimer(WaveSpawnTimerHandle);
}

void AZombieWaveManager::TriggerNextWave()
{
	CurrentWaveIndex++;

	// todo: load new wave data, increase difficulty, etc
	ZombiesToSpawn += 5;

	// reset wave variables
	zombiesSpawned = 0;
	ZombiesKilled = 0;

	// Update wave UI for all players
	if (PlayerUIManagerRef)
	{
		PlayerUIManagerRef->UpdateWave(GetCurrentWave());
	}
	
	GetWorld()->GetTimerManager().SetTimer(WaveStartCooldownTimerHandle, this, &AZombieWaveManager::StartWaveSpawner,
	                                       WaveStartCooldown, false);
}
