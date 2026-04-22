// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/_Game/GameModes/FpsGameMode.h"

#include "CodZombiesClone.h"
#include "_Game/Characters/PlayerCharacter.h"

AFpsGameMode::AFpsGameMode()
{
	// Stub
}

TArray<APlayerCharacter*> AFpsGameMode::GetAlivePlayerActors()
{
	TArray<APlayerCharacter*> AlivePlayerActors;

	for (APlayerCharacter* player : PlayerActors)
	{
		if (!player->GetIsDowned())
		{
			AlivePlayerActors.Add(player);
		}
	}

	return AlivePlayerActors;
}
