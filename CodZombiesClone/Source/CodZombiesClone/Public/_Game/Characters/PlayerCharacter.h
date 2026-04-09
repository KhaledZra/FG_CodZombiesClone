// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "_Game/UI/PlayerUI.h"
#include "_Game/Weapons/WeaponUser.h"
#include "PlayerCharacter.generated.h"

class ABaseWeapon;

UCLASS()
class CODZOMBIESCLONE_API APlayerCharacter : public ABaseCharacter, public IWeaponUser
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UFUNCTION(BlueprintCallable)
	void GainScore(int Score);
	
	int GetCurrentScore() const;
	
	void DoLeftFireStarted();
	
	void SetupPlayer(APlayerController* OwningController, FColor PlayerColor, int CurrentPlayerIndex);

protected:
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UPlayerUI> PlayerUiClass;
	
	UPROPERTY()
	TObjectPtr<UPlayerUI> PlayerUIRef;
	
	UPROPERTY(EditAnywhere, Category="Score")
	int StartingScore;
	
	UPROPERTY()
	int CurrentScore;
	
	int PlayerIndex;

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName FpsWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName TpsWeaponSocket = FName("HandGrip_R");

	UPROPERTY(EditAnywhere, Category="Weapons")
	TObjectPtr<ABaseWeapon> CurrentWeapon;

	UPROPERTY(EditAnywhere, Category="Weapons")
	TSubclassOf<ABaseWeapon> StarterWeapon;

	/** IWeaponUser implementation */
	virtual void AttachWeapon(ABaseWeapon* Weapon) override;
	virtual void OnWeaponActivated(ABaseWeapon* Weapon) override;
	virtual void OnWeaponDeactivated(ABaseWeapon* Weapon) override;
	virtual void PlayWeaponFireMontage(UAnimMontage* Montage) override;
	virtual void EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass) override;
	virtual void GetTargetAimLocation(FVector& OutStartLocation, FVector& OutWorldDirection) override;
	virtual void UpdateWeaponHud(int CurrentAmmo, int MagazineSize) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void BP_PlayFpsAnimMontage(UAnimMontage* Montage);
	
	void CreatePlayerUI(APlayerController* OwningController, int CurrentPlayerIndex);
	void SetPlayerIndex(int Index);
	void SetPlayerColor(FColor Color) const;
};
