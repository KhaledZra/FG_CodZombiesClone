// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class CODZOMBIESCLONE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* FpsMesh;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* TpsMesh;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	int GetMagazineSize() const;

	UPROPERTY(VisibleAnywhere, Category= "Ammo")
	int CurrentAmmo;

protected:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle WeaponData;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
