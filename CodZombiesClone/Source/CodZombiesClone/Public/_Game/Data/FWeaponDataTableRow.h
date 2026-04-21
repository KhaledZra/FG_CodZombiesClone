#pragma once

#include "FWeaponDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int MagazineSize = 0;

	UPROPERTY(EditAnywhere)
	int StartingReserveAmmoCount = 0;
	
	UPROPERTY(EditAnywhere)
	float RecoilStrength = 0;
	
	UPROPERTY(EditAnywhere)
	float FireRate = 0;

	UPROPERTY(EditAnywhere)
	float BulletRange = 0;
	
	UPROPERTY(EditAnywhere)
	int GunDamage = 0;
	
	UPROPERTY(EditAnywhere)
	float ReloadLength = 0;
	
	UPROPERTY(EditAnywhere)
	bool bAutoFire = false;
	
	UPROPERTY(EditAnywhere)
	bool bShotgunSpread = false;
	
	UPROPERTY(EditAnywhere)
	int SpreadCount = 0;
	
	UPROPERTY(EditAnywhere)
	float MaxSpreadDegree = 0;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> TpsWeaponMesh;
	
	UPROPERTY(EditAnywhere, Category="Player")
	TSoftObjectPtr<UAnimMontage> FiringMontage;
	
	UPROPERTY(EditAnywhere, Category="Player")
	TSoftObjectPtr<UAnimMontage> ReloadMontage;
		
	UPROPERTY(EditAnywhere, Category="Player")
	TSoftObjectPtr<UAnimMontage> EquippedMontage;
	
	UPROPERTY(EditAnywhere, Category="Player")
	TSoftObjectPtr<UAnimMontage> DryFireMontage;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSoftObjectPtr<UAnimMontage> WeaponReloadMontage;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSoftObjectPtr<UAnimMontage> WeaponFireMontage;
	
	UPROPERTY(EditAnywhere, Category="Player")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	
	UPROPERTY(EditAnywhere, Category="Player")
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<UAnimInstance> WeaponAnimInstanceClass;
};
