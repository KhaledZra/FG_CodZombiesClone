// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class CODZOMBIESCLONE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<AActor> CurrentTarget;
protected:
	
	FTimerHandle SetTargetTimer;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;
};
