// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Managers/PlayerUIManager.h"


// Sets default values
APlayerUIManager::APlayerUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APlayerUIManager::AddPlayerUI(UPlayerUI* PlayerUI)
{
	PlayerUis.Add(PlayerUI);
	UpdateUI(PlayerUI); // Basically telling other players that a new player has arrived and they should add a new UI for them
}

void APlayerUIManager::UpdateUI(UPlayerUI* CallingUI)
{
	for (UPlayerUI* ui : PlayerUis)
	{
		if (ui == CallingUI) continue;
		
		// ui->RegisterNewPlayerUI(CallingUI);
	}
}

// Called when the game starts or when spawned
void APlayerUIManager::BeginPlay()
{
	Super::BeginPlay();
	
}

