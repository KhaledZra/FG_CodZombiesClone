#pragma once

#include "FWeaponDataTableRow.generated.h"

struct FWeaponStats;

USTRUCT(BlueprintType)
struct FWeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TArray<FWeaponStats> WeaponStatsArray;

	UPROPERTY(EditAnywhere, Category="Player")
	TSoftObjectPtr<USkeletalMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere, Category="Player")
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
