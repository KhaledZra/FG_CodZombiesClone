// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/UI/PlayerUI.h"

void UPlayerUI::UpdateScore(int NewScore)
{
	BP_ScoreUpdated(NewScore);
	
	// // Tell the manager that this UI has been updated so it can update the other UIs
	// if (APlayerUIManager* Manager = Cast<APlayerUIManager>(GetWorld()->GetAuthGameMode()))
	// {
	// 	Manager->UpdateUI(this);
	// }
}

void UPlayerUI::OtherPlayerScoreUpdated(int NewScore, UPlayerUI* CallingUI)
{
	// Find the UI element that displays the other player's score and update it
	// BP_OtherPlayerScoreUpdated(NewScore);
}
