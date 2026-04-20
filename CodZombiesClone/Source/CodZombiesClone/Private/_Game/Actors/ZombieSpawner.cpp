// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Actors/ZombieSpawner.h"

#include "CodZombiesClone.h"
#include "_Game/Characters/ZombieCharacter.h"


// Sets default values
AZombieSpawner::AZombieSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AZombieSpawner::TrySpawnZombie(const TSubclassOf<AZombieCharacter>& ZombieClass)
{
	if (ZombieClass == nullptr) return false;
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AZombieCharacter* CurrentSpawnedActor = GetWorld()->SpawnActor<AZombieCharacter>(ZombieClass, GetActorLocation(), FRotator::ZeroRotator,
														 SpawnParameters);
	if (CurrentSpawnedActor == nullptr)
	{
		UE_LOG(Khaled, Error, TEXT("AZombieSpawner::TrySpawnZombie - Failed To Spawn Actor"));
		return false;
	}
	
	CurrentSpawnedActor->OnDestroyed.AddDynamic(this, &AZombieSpawner::NotifyZombieKilled);
	return true;
}

void AZombieSpawner::NotifyZombieKilled(AActor* ZombieActor)
{
	OnZombieKilled.Broadcast();
}

