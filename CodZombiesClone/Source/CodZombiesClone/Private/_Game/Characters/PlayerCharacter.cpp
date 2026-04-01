// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Characters/PlayerCharacter.h"

#include "CodZombiesClone.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StartingScore = 500;
	CurrentScore = 0;
}

void APlayerCharacter::CreatePlayerUI(APlayerController* OwningController)
{
	PlayerUIRef = CreateWidget<UPlayerUI>(OwningController, PlayerUiClass);
	ensure(PlayerUIRef != nullptr);
	PlayerUIRef->AddToPlayerScreen();
	
	// UE_LOG(Khaled, Warning, TEXT("Widget instance: %p | Controller: %s"),
	// 	PlayerUIRef,
	// 	*this->GetName());
	
	GainScore(StartingScore);
}

void APlayerCharacter::GainScore(int Score)
{
	CurrentScore += Score;
	if (PlayerUIRef) PlayerUIRef->UpdateScore(CurrentScore);
}

