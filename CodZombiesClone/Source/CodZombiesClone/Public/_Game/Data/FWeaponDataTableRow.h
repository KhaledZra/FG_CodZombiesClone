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
	TSoftObjectPtr<USkeletalMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> TpsWeaponMesh;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> FiringMontage;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> ReloadMontage;
		
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> EquippedMontage;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
};
