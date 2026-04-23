#pragma once

#include "FWeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int MagazineSize = 0;

	UPROPERTY(EditAnywhere)
	int StartingReserveAmmoCount = 0; // not used
	
	UPROPERTY(EditAnywhere)
	float RecoilStrength = 0;
	
	UPROPERTY(EditAnywhere)
	float FireRate = 0;

	UPROPERTY(EditAnywhere)
	float BulletRange = 0;
	
	UPROPERTY(EditAnywhere)
	int GunDamage = 0;
	
	UPROPERTY(EditAnywhere)
	float ReloadSpeedMultiplier = 0;
	
	UPROPERTY(EditAnywhere)
	bool bAutoFire = false;
	
	UPROPERTY(EditAnywhere, Category="Shotgun")
	bool bShotgunSpread = false;
	
	UPROPERTY(EditAnywhere, Category="Shotgun")
	int SpreadCount = 0;
	
	UPROPERTY(EditAnywhere, Category="Shotgun")
	float MaxSpreadDegree = 0;
};