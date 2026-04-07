// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "_Game/UI/PlayerUI.h"
#include "PlayerCharacter.generated.h"

class ABaseWeapon;

UCLASS()
class CODZOMBIESCLONE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
	UFUNCTION()
	void CreatePlayerUI(APlayerController* OwningController);
	
	UFUNCTION(BlueprintCallable)
	void GainScore(int Score);
	
	void SetPlayerIndex(int Index);
	
	int GetCurrentScore() const;
	
	void SetPlayerColor(FColor Color) const;

protected:
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UPlayerUI> PlayerUiClass;
	
	UPROPERTY()
	UPlayerUI* PlayerUIRef;
	
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
};
