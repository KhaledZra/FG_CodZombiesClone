// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "_Game/UI/PlayerUI.h"
#include "PlayerCharacter.generated.h"

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
};
