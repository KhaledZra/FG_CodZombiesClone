// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Characters/PlayerCharacter.h"

#include "CodZombiesClone.h"
#include "Camera/CameraComponent.h"
#include "_Game/Weapons/BaseWeapon.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StartingScore = 500;
	CurrentScore = StartingScore;
}

void APlayerCharacter::CreatePlayerUI(APlayerController* OwningController, int CurrentPlayerIndex)
{
	PlayerUIRef = CreateWidget<UPlayerUI>(OwningController, PlayerUiClass);
	ensure(PlayerUIRef != nullptr);
	PlayerUIRef->AddToPlayerScreen();
	
	PlayerUIRef->SetupPlayerUI(CurrentPlayerIndex, this);
	PlayerUIRef->UpdateScore(CurrentScore, CurrentPlayerIndex);
	
	// UE_LOG(Khaled, Warning, TEXT("Widget instance: %p | Controller: %s"),
	// 	&PlayerUIRef,
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

void APlayerCharacter::SetPlayerColor(const FColor Color) const
{
	GetMesh()->SetColorParameterValueOnMaterials("Paint Tint", Color);
	FirstPersonMesh->SetColorParameterValueOnMaterials("Paint Tint", Color);
}

void APlayerCharacter::DoLeftFireStarted()
{
	// Holding weapon check
	if (!CurrentWeapon) return;
	
	//todo: work in progress should fire from weapon
	CurrentWeapon->StartFiring();
}

void APlayerCharacter::SetupPlayer(APlayerController* OwningController, FColor PlayerColor, int CurrentPlayerIndex)
{
	CreatePlayerUI(OwningController, CurrentPlayerIndex);
	SetPlayerIndex(CurrentPlayerIndex);
	SetPlayerColor(PlayerColor);
	if (StarterWeapon) EquipWeapon(StarterWeapon);
}

// Right now this is called inside the BaseWeapon BeginPlay
void APlayerCharacter::AttachWeapon(ABaseWeapon* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	
	Weapon->AttachToActor(this, AttachmentRule);
	Weapon->GetFirstPersonMesh()->AttachToComponent(FirstPersonMesh, AttachmentRule, FpsWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, TpsWeaponSocket);
}

void APlayerCharacter::OnWeaponActivated(ABaseWeapon* Weapon)
{
	// todo: Does nothing
}

void APlayerCharacter::OnWeaponDeactivated(ABaseWeapon* Weapon)
{
	// todo: Does nothing
}

void APlayerCharacter::PlayWeaponFireMontage(UAnimMontage* Montage)
{
	// todo: Needs to also trigger the TpsAnimMontage
	BP_PlayFpsAnimMontage(Montage);
}

void APlayerCharacter::EquipWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	// spawn the new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	ABaseWeapon* AddedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetActorTransform(), SpawnParams);

	if (!AddedWeapon)
	{
		UE_LOG(Khaled, Error, TEXT("APlayerCharacter::EquipWeapon - Failed to spawn weapon"));
		return;
	}

	// Use the new weapon
	CurrentWeapon = AddedWeapon;
	CurrentWeapon->SetActorHiddenInGame(false);

	// set the character mesh AnimInstances
	GetFirstPersonMesh()->SetAnimInstanceClass(CurrentWeapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(CurrentWeapon->GetThirdPersonAnimInstanceClass());
}

void APlayerCharacter::GetTargetAimLocation(FVector& OutStartLocation, FVector& OutWorldDirection)
{
	OutStartLocation = FirstPersonCameraComponent->GetComponentLocation();
	OutWorldDirection = FirstPersonCameraComponent->GetForwardVector();
}

void APlayerCharacter::UpdateWeaponHud(int CurrentAmmo, int MagazineSize)
{
	if (PlayerUIRef)
	{
		PlayerUIRef->BP_UpdateBulletCounter(MagazineSize, CurrentAmmo);
		
		// UE_LOG(Khaled, Warning, TEXT("Widget instance: %p | Controller: %s"),
		// 	&PlayerUIRef,
		// 	*this->GetName());
		UE_LOG(Khaled, Warning, TEXT("Updated Ammo for %s"), *GetName());
		// Ammo logs
		UE_LOG(Khaled, Warning, TEXT("Current Ammo: %d | Magazine Size: %d"), CurrentAmmo, MagazineSize);
	}
	else
	{
		UE_LOG(Khaled, Warning, TEXT("UI ref Missing for PlayerCharacter %s"), *GetName());
	}
}

