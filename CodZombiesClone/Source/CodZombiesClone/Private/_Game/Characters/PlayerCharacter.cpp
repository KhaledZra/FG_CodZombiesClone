// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Characters/PlayerCharacter.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StartingScore = 500;
	CurrentScore = StartingScore;
}

void APlayerCharacter::CreatePlayerUI(APlayerController* OwningController)
{
	PlayerUIRef = CreateWidget<UPlayerUI>(OwningController, PlayerUiClass);
	ensure(PlayerUIRef != nullptr);
	PlayerUIRef->AddToPlayerScreen();
	
	PlayerUIRef->SetupPlayerUI(PlayerIndex, this);
	PlayerUIRef->UpdateScore(CurrentScore, PlayerIndex);
	
	// UE_LOG(Khaled, Warning, TEXT("Widget instance: %p | Controller: %s"),
	// 	PlayerUIRef,
	// 	*this->GetName());
}

void APlayerCharacter::GainScore(int Score)
{
	CurrentScore += Score;
	if (PlayerUIRef) PlayerUIRef->UpdateScore(CurrentScore, PlayerIndex);
}

void APlayerCharacter::SetPlayerIndex(int Index)
{
	this->PlayerIndex = Index;
}

int APlayerCharacter::GetCurrentScore() const
{
	return CurrentScore;
}

