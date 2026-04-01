// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * The base UI for the player
 */
UCLASS()
class CODZOMBIESCLONE_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<UPlayerUI*> OtherPlayerUis;
	
	UFUNCTION()
	void UpdateScore(int NewScore);
	
	UFUNCTION()
	void OtherPlayerScoreUpdated(int NewScore, UPlayerUI* CallingUI);
	
	// Let's the UI BP handle the updating of the widget
	UFUNCTION(BlueprintImplementableEvent, Category="Player", meta = (DisplayName = "Score Updated"))
	void BP_ScoreUpdated(int Score);
};
