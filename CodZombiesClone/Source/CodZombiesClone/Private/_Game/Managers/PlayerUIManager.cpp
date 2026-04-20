// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Managers/PlayerUIManager.h"

#include "CodZombiesClone.h"
#include "_Game/Characters/PlayerCharacter.h"
#include "_Game/UI/PlayerUI.h"


// Sets default values
APlayerUIManager::APlayerUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerUIManager::AddPlayerUI(UPlayerUI* PlayerUI, int PlayerIndex)
{
	PlayerUis.Add(PlayerUI);

	// The reason this is so fucked up is because we need to maintain the order of the players that are already joined
	// If we are adding PlayerIndex 3 it's assumed that there are already 3 players in the game, so we need to activate all of them up to the new one
	for (int i = 0; i < PlayerIndex + 1; ++i)
	{
		// Makes sure old players are active and the new one is active as well
		for (UPlayerUI* ui : PlayerUis)
		{
			ui->SetPlayerActive(i);
		}
		
		// Syncs the real current score of old players with the new player
		PlayerUI->BP_ScoreUpdated(PlayerUis[i]->PlayerRef->GetCurrentScore(), i);
	}
	
	// Sync player with current wave
	PlayerUI->BP_WaveUpdated(CurrentWaveCache);
}

void APlayerUIManager::UpdateUI(int NewScore, int PlayerIndex)
{
	for (UPlayerUI* ui : PlayerUis)
	{
		ui->BP_ScoreUpdated(NewScore, PlayerIndex);
	}
}

void APlayerUIManager::UpdateWave(int NewWave)
{
	for (UPlayerUI* ui : PlayerUis)
	{
		ui->BP_WaveUpdated(NewWave);
	}
	
	// cache last wave for new players
	CurrentWaveCache = NewWave;
}
