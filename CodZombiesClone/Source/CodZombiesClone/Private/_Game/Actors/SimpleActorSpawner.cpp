// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Actors/SimpleActorSpawner.h"


// Sets default values
ASimpleActorSpawner::ASimpleActorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASimpleActorSpawner::SpawnActor(AActor* OldSpawnedActor)
{
	if (ActorClassToSpawn == nullptr) return;
	// if (OldSpawnedActor != nullptr) return; // Not dead?
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	CurrentSpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClassToSpawn, GetActorLocation(), FRotator::ZeroRotator,
	                                                     SpawnParameters);
	CurrentSpawnedActor->OnDestroyed.AddDynamic(this, &ASimpleActorSpawner::SpawnActor);
	// todo: fix Context Requirements in UStateTreeComponent::StopLogic failed. Component tick is disabled.
}

// Called when the game starts or when spawned
void ASimpleActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnActor(nullptr);
}
