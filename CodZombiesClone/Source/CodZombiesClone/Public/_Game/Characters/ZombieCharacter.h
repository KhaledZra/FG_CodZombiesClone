// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "ZombieCharacter.generated.h"

class AEnemyAIController;
class UStateTreeAIComponent;

UCLASS()
class CODZOMBIESCLONE_API AZombieCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	TSubclassOf<AEnemyAIController> EnemyAIController;
	
public:
	// Sets default values for this character's properties
	AZombieCharacter();

	UFUNCTION(BlueprintCallable)
	void AttackForward();

protected:
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackRange;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackSphereRadius;

	UPROPERTY(EditAnywhere, Category="Attack")
	int AttackDamage;

	UPROPERTY(EditAnywhere, Category="Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void BP_PlayAnimMontage(UAnimMontage* Montage);
	
	FTimerHandle OnDeathTimerHandle;

protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
};
