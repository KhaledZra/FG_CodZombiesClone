// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleActorSpawner.generated.h"

UCLASS()
class CODZOMBIESCLONE_API ASimpleActorSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimpleActorSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClassToSpawn;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentSpawnedActor;

	UFUNCTION(BlueprintCallable)
	void SpawnActor(AActor* OldSpawnedActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
