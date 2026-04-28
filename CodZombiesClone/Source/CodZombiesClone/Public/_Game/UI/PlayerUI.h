// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

class APlayerCharacter;
class APlayerUIManager;
/**
 * The base UI for the player
 */
UCLASS()
class CODZOMBIESCLONE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Manager")
	TObjectPtr<APlayerUIManager> ManagerRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Manager")
	TObjectPtr<APlayerCharacter> PlayerRef;
	
	UFUNCTION()
	void UpdateScore(int NewScore, int PlayerIndex);
	
	UFUNCTION()
	void SetupPlayerUI(int PlayerIndex, APlayerCharacter* Player);
	
	UFUNCTION()
	void SetPlayerActive(int PlayerIndex);
	
	
	// Let's the UI BP handle the updating of the widget
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta = (DisplayName = "Score Updated"))
	void BP_ScoreUpdated(int Score, int PlayerIndex);
	
	// Let's the UI BP handle the updating of the widget
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta = (DisplayName = "Sets the main player"))
	void BP_SetMainPlayer(int PlayerIndex);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta = (DisplayName = "Activates the given Index"))
	void BP_ActivatePlayer(int PlayerIndex);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "UpdateBulletCounter"))
	void BP_UpdateBulletCounter(int MagazineSize, int BulletCount);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "UpdateHealthBar"))
	void BP_UpdateHealthBar(int CurrentHealth, int MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "Shot Fired"))
	void BP_ShotFired();

	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "Update Interaction String"))
	void BP_UpdateInteractionText(const FString& InteractionString);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "Update Wave"))
	void BP_WaveUpdated(const int NewWave);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta=(DisplayName = "On Target Hit"))
	void BP_OnTargetHit(const bool bIsKill);
};
