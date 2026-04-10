// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


class IHealthUser;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CODZOMBIESCLONE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

	void SetMaxHealth(const int& Health, const bool& bForceHeal = true);

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(const int& HealAmount);

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(const int& DamageAmount);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void Die();

protected:
	IHealthUser* HealthUser;

	// todo: Maybe load using DT in future

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int CurrentHealth;

	UPROPERTY(EditAnywhere, Category="Health")
	int StartingMaxHealth;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
