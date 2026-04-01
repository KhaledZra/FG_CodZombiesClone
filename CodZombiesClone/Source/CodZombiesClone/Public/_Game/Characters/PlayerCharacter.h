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
	
	UFUNCTION()
	void GainScore(int Score);
	
protected:
	
	UPROPERTY(EditAnywhere, Category ="UI")
	TSubclassOf<UPlayerUI> PlayerUiClass;
	
	UPROPERTY(VisibleAnywhere, Category ="UI")
	UPlayerUI* PlayerUIRef;
	
	UPROPERTY(blueprintreadwrite)
	int StartingScore;
	
	UPROPERTY()
	int CurrentScore;
};
