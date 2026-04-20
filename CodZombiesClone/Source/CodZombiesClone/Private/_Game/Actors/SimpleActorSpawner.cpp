// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Actors/SimpleActorSpawner.h"

#include "CodZombiesClone.h"


// Sets default values
ASimpleActorSpawner::ASimpleActorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleActorSpawner::SpawnActor(AActor* OldSpawnedActor)
{
	if (ActorClassToSpawn == nullptr) return;
	// use .IsValid cause they wont be nullptr at this stage
	// if (OldSpawnedActor != nullptr) return; // Not dead?
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CurrentSpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClassToSpawn, GetActorLocation(), FRotator::ZeroRotator,
	                                                     SpawnParameters);
	if (CurrentSpawnedActor == nullptr)
	{
		UE_LOG(Khaled, Error, TEXT("ASimpleActorSpawner::SpawnActor - Failed To Spawn Actor"));
		return;
	}
	
	CurrentSpawnedActor->OnDestroyed.AddDynamic(this, &ASimpleActorSpawner::SpawnActor);
}

// Called when the game starts or when spawned
void ASimpleActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnActor(nullptr);
}
