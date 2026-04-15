// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UStateTreeAIComponent;

UCLASS()
class CODZOMBIESCLONE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Component)
	TObjectPtr<UStateTreeAIComponent> StateTree;

public:
	// Sets default values for this actor's properties
	AEnemyAIController();

	UFUNCTION(BlueprintCallable)
	void StartAILogic();

	UFUNCTION(BlueprintCallable)
	void StopAILogic();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
