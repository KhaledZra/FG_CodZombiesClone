// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class IWeaponUser;

UCLASS()
class CODZOMBIESCLONE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category="Components")
	USkeletalMeshComponent* FpsMesh;

	UPROPERTY(EditAnywhere, Category="Components")
	USkeletalMeshComponent* TpsMesh;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	UPROPERTY(VisibleAnywhere, Category= "Ammo")
	int CurrentAmmo;
	
	int GetMagazineSize() const;

	USkeletalMeshComponent* GetFirstPersonMesh() const { return FpsMesh; }
	USkeletalMeshComponent* GetThirdPersonMesh() const { return TpsMesh; }

	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const { return FirstPersonAnimInstanceClass; }
	const TSubclassOf<UAnimInstance>& GetThirdPersonAnimInstanceClass() const { return ThirdPersonAnimInstanceClass; }

protected:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle WeaponData;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category="Animation")
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
	
	IWeaponUser* WeaponUser;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
