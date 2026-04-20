// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Managers/ZombieWaveManager.h"

#include "CodZombiesClone.h"
#include "_Game/Actors/ZombieSpawner.h"


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
		// todo: trigger new wave?
		// update hud with new wave info?
	}
}

void AZombieWaveManager::BeginPlay()
{
	Super::BeginPlay();

	// Get all zombie spawners, apparently TObjectIterator is better than TActorIterator???
	for (TObjectIterator<AZombieSpawner> Itr; Itr; ++Itr)
	{
		// since TObjectIterator does not filter by world this is needed, gg
		if (Itr->GetWorld() != GetWorld()) continue;
		
		Itr->OnZombieKilled.AddDynamic(this, &AZombieWaveManager::OnZombieKilled);
		ZombieSpawners.Add(*Itr);
	}
	
	if (ZombieSpawners.IsEmpty())
	{
		UE_LOG(Khaled, Error, TEXT("AZombieWaveManager::BeginPlay - No Zombie Spawners found in the world!"));
	}
	else
	{
		UE_LOG(Khaled, Warning, TEXT("AZombieWaveManager::BeginPlay - Wave Started!"));
		bWaveActive = true;
	}
}

void AZombieWaveManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!bWaveActive) return;
	// todo: slow it down, float spawnSpeed;
	
	AZombieSpawner* rndSpawner = ZombieSpawners[FMath::RandRange(0, ZombieSpawners.Num() - 1)];
	bool bIsSpawned = rndSpawner->TrySpawnZombie(ZombieClass);

	if (!bIsSpawned) return;
		
	zombiesSpawned++;

	if (zombiesSpawned >= ZombiesToSpawn)
	{
		bWaveActive = false;
	}
}
