// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/AIController/EnemyAIController.h"

#include "CodZombiesClone.h"
#include "Components/StateTreeAIComponent.h"


// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTree"));
	StateTree->SetStartLogicAutomatically(false);
}

void AEnemyAIController::StartAILogic()
{
	StateTree->StartLogic();
}

void AEnemyAIController::StopAILogic()
{
	StateTree->StopLogic("Stopping");
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(Khaled, Log, TEXT("Possessing now"));

	StartAILogic();
}
