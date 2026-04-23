// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "_Game/Interfaces/InteractionUser.h"
#include "_Game/UI/PlayerUI.h"
#include "_Game/Interfaces/WeaponUser.h"
#include "PlayerCharacter.generated.h"

class AFpsPlayerController;
class UInteractionComponent;
class ABaseWeapon;

USTRUCT()
struct FStarterWeapon
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle WeaponData;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;
};

UCLASS()
class CODZOMBIESCLONE_API APlayerCharacter : public ABaseCharacter, public IWeaponUser, public IInteractionUser
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInteractionComponent> InteractionComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UFUNCTION(BlueprintCallable)
	void GainScore(int Score);
	
	UFUNCTION(BlueprintCallable)
	bool TryUseScore(int Score);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentScore() const;
	
	void DoLeftFireStarted();
	void DoLeftFireStopped();
	void DoReload();
	void DoInteractionStarted();
	void OnInteractionOngoing();
	void DoInteractionStopped();
	void DoCycleWeapons();
	
	void SetupPlayer(APlayerController* OwningController, FColor PlayerColor, int CurrentPlayerIndex);

	bool GetIsDowned() const;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsDowned = false;

	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName FpsWeaponSocket = FName("HandGrip_R");

	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName TpsWeaponSocket = FName("HandGrip_R");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapons")
	TObjectPtr<ABaseWeapon> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapons")
	TArray<TObjectPtr<ABaseWeapon>> OwnedWeapons;

	UPROPERTY(EditAnywhere, Category="Weapons")
	TArray<FStarterWeapon> StarterWeapons;
	
	FTimerHandle EquipWeaponVisibilityTimer;
	
	UPROPERTY()
	TObjectPtr<AFpsPlayerController> FpsControllerRef;

	/** IWeaponUser implementation */
	virtual void AttachWeapon(ABaseWeapon* Weapon) override;
	virtual void OnWeaponActivated(ABaseWeapon* Weapon) override;
	virtual void OnWeaponDeactivated(ABaseWeapon* Weapon) override;
	virtual void PlayWeaponMontage(UAnimMontage* Montage, float PlayRate = 1.0f) override;
	UFUNCTION(BlueprintCallable)
	virtual bool TryEquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass, FDataTableRowHandle WeaponData) override;
	virtual void GetTargetAimLocation(FVector& OutStartLocation, FVector& OutWorldDirection) override;
	virtual void UpdateWeaponHud(int CurrentAmmo, int MagazineSize) override;
	virtual void AddRecoil(float RecoilStrength) override;
	virtual void OnEnemyHit(int Points) override;
	virtual void OnShotFired() override;

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void BP_PlayFpsAnimMontage(UAnimMontage* Montage, float PlayRate = 1.0f);

	UFUNCTION(BlueprintImplementableEvent, Category="Animation")
	void BP_PlayDeathAnimMontage();
	
	void CreatePlayerUI(APlayerController* OwningController, int CurrentPlayerIndex);
	void SetPlayerIndex(int Index);
	void SetPlayerColor(FColor Color) const;
	void SpawnStarterWeapons();
	bool IsWeaponAlreadyOwned(const FDataTableRowHandle& WeaponData) const;
	
	// IHealthUser interface
	virtual void OnDeath() override;
	virtual void OnHealthUIUpdate(const int& CurrentHealth, const int& MaxHealth) override;
	
	// IInteractionUser interface
	virtual void OnUpdateInteractionUI(const FString& InteractString) override;
	virtual FText GetInteractionKeyText() override;
};
