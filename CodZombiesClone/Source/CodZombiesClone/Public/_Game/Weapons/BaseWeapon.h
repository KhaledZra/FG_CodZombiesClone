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
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	int CurrentAmmo;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	int MagazineSize;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	float RecoilStrength;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	float FireRate;

	UPROPERTY(VisibleAnywhere, Category= "Data")
	float BulletRange;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	int GunDamage;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	bool bAutoFire;
	
	UPROPERTY(VisibleAnywhere, Category= "Data")
	float ReloadLength;
	
	void StartFiring();
	void StopFiring();
	void Reload();
	void ActivateWeapon();

	UFUNCTION(BlueprintCallable, Category="Mesh")
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FpsMesh; }
	UFUNCTION(BlueprintCallable, Category="Mesh")
	USkeletalMeshComponent* GetThirdPersonMesh() const { return TpsMesh; }

	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const { return FirstPersonAnimInstanceClass; }
	const TSubclassOf<UAnimInstance>& GetThirdPersonAnimInstanceClass() const { return ThirdPersonAnimInstanceClass; }

protected:
	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle WeaponData;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> FiringMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> EquippedMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> DryFireMontage;
	
	// weapon anims
	UPROPERTY()
	TObjectPtr<UAnimMontage> WeaponReloadMontage;
	
	UPROPERTY()
	TObjectPtr<UAnimMontage> WeaponFireMontage;
	
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
	
	// TScriptInterface if we need it in BP
	IWeaponUser* WeaponUser;
	
	bool bFireCooldownActive = false;
	bool bIsReloading = false;
	FTimerHandle FireCooldownTimer;
	FTimerHandle AutoFireTimer;
	FTimerHandle ReloadTimer;

	UFUNCTION()
	void OnOwnerDestroyed(AActor* DestroyedActor);
	
	void Fire();
	void OnReloadComplete();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void BP_PlayAnimMontage(UAnimMontage* Montage);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
};
