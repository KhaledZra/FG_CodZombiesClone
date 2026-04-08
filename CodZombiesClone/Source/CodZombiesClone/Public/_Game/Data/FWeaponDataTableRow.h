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
	TSoftObjectPtr<USkeletalMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> TpsWeaponMesh;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAnimMontage> FiringMontage;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;
};
