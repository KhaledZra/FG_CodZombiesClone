// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/AIController/EnemyAIController.h"

#include "_Game/GameModes/FpsGameMode.h"


// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	GetWorld()->GetTimerManager().SetTimer(SetTargetTimer, FTimerDelegate::CreateLambda([this]
	{
		if (AFpsGameMode* gm = GetWorld()->GetAuthGameMode<AFpsGameMode>())
		{
			// todo: Not super clean maybe have some sort of manager that feeds a target on start?
			CurrentTarget = gm->PlayerActors[FMath::RandRange(0, gm->PlayerActors.Num() - 1)];
		}
	}), 5.0f, false);
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
	CurrentTarget = nullptr;
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}
