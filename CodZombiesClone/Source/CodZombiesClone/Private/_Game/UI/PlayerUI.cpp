// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/UI/PlayerUI.h"

#include "CodZombiesClone.h"
#include "Kismet/GameplayStatics.h"
#include "_Game/Managers/PlayerUIManager.h"

void UPlayerUI::UpdateScore(int NewScore, int PlayerIndex)
{
	if (ManagerRef) ManagerRef->UpdateUI(NewScore, PlayerIndex);
}

void UPlayerUI::SetupPlayerUI(int PlayerIndex, APlayerCharacter* Player)
{
	PlayerRef = Player;
	
	if (AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerUIManager::StaticClass()))
	{
		ManagerRef = Cast<APlayerUIManager>(actor);
		if (ManagerRef)
		{
			ManagerRef->AddPlayerUI(this, PlayerIndex);
		}
		else
		{
			UE_LOG(Khaled, Error, TEXT("Found PlayerUIManager but couldn't cast it! | %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(Khaled, Error, TEXT("Failed to find PlayerUIManager! | %s"), *GetName());
	}
	
	BP_SetMainPlayer(PlayerIndex);
}

void UPlayerUI::SetPlayerActive(int PlayerIndex)
{
	BP_ActivatePlayer(PlayerIndex);
}
