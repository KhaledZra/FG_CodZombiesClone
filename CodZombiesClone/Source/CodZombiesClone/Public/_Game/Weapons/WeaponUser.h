// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponUser.generated.h"

class ABaseWeapon;
// This class does not need to be modified.
UINTERFACE()
class UWeaponUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * This inferface just cleans up and abstracts weapon logic between whoever can use a weapon and the weapon it self
 */
class CODZOMBIESCLONE_API IWeaponUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttachWeapon(ABaseWeapon* Weapon) = 0;
	virtual void OnWeaponActivated(ABaseWeapon* Weapon) = 0;
	virtual void OnWeaponDeactivated(ABaseWeapon* Weapon) = 0;
	virtual void PlayWeaponFireMontage(UAnimMontage* Montage) = 0;
	virtual void EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass) = 0;
	virtual void GetTargetAimLocation(FVector& OutStartLocation, FVector& OutWorldDirection) = 0;
	virtual void UpdateWeaponHud(int CurrentAmmo, int MagazineSize) = 0;
	virtual void AddRecoil(float RecoilStrength) = 0;
};
