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
	TObjectPtr<USkeletalMeshComponent> FpsMesh;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USkeletalMeshComponent> TpsMesh;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	UPROPERTY(VisibleAnywhere, Category= "Ammo")
	int CurrentAmmo;
	
	UPROPERTY(VisibleAnywhere, Category= "Ammo")
	int MagazineSize;
	
	void StartFiring();

	USkeletalMeshComponent* GetFirstPersonMesh() const { return FpsMesh; }
	USkeletalMeshComponent* GetThirdPersonMesh() const { return TpsMesh; }

	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const { return FirstPersonAnimInstanceClass; }
	const TSubclassOf<UAnimInstance>& GetThirdPersonAnimInstanceClass() const { return ThirdPersonAnimInstanceClass; }

protected:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle WeaponData;

	UPROPERTY()
	TObjectPtr<UAnimMontage> FiringMontage;
	
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
	
	// TScriptInterface if we need it in BP
	IWeaponUser* WeaponUser;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
