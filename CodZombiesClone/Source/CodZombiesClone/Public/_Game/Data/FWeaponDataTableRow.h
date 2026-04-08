#pragma once

#include "FWeaponDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int CurrentMagazineSize = 0;

	UPROPERTY(EditAnywhere)
	int CurrentReserveAmmoCount = 0;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> TpsWeaponMesh;
};
