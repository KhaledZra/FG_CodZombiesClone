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
	TSoftObjectPtr<UStaticMesh> FpsWeaponMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UStaticMesh> TpsWeaponMesh;
};
