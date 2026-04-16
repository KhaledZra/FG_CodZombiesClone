// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Characters/PlayerCharacter.h"

#include "CodZombiesClone.h"
#include "Camera/CameraComponent.h"
#include "_Game/Components/InteractionComponent.h"
#include "_Game/Weapons/BaseWeapon.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

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
	PlayerUIRef->BP_UpdateInteractionText("");

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

void APlayerCharacter::OnDeath()
{
	// todo: death stuff? maybe go to downed state?
}

void APlayerCharacter::OnHealthUIUpdate(const int& CurrentHealth, const int& MaxHealth)
{
	if (PlayerUIRef)
	{
		PlayerUIRef->BP_UpdateHealthBar(CurrentHealth, MaxHealth);
	}
}

void APlayerCharacter::OnUpdateInteractionUI(const FString& InteractString)
{
	if (PlayerUIRef)
	{
		PlayerUIRef->BP_UpdateInteractionText(InteractString);
	}
}

void APlayerCharacter::DoLeftFireStarted()
{
	// Holding weapon check
	if (!CurrentWeapon) return;
	
	CurrentWeapon->StartFiring();
}

void APlayerCharacter::DoLeftFireStopped()
{
	// Holding weapon check
	if (!CurrentWeapon) return;
	
	CurrentWeapon->StopFiring();
}

void APlayerCharacter::DoReload()
{
	if (!CurrentWeapon) return;
	
	CurrentWeapon->Reload();
}

void APlayerCharacter::SetupPlayer(APlayerController* OwningController, FColor PlayerColor, int CurrentPlayerIndex)
{
	CreatePlayerUI(OwningController, CurrentPlayerIndex);
	SetPlayerIndex(CurrentPlayerIndex);
	SetPlayerColor(PlayerColor);
	if (StarterWeapon) EquipWeapon(StarterWeapon);
	InteractionComponent->StartInteractionSystem(FirstPersonCameraComponent);
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
	UE_LOG(Khaled, Log, TEXT("Weapon activated: %s"), *Weapon->GetName());

	// set the character mesh AnimInstances
	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
	
	// Use the new weapon
	Weapon->ActivateWeapon();
	
	CurrentWeapon = Weapon;
	// Mini delay to make it look better when equipping, super hacky but w/e
	GetWorld()->GetTimerManager().SetTimer(EquipWeaponVisibilityTimer,
								   FTimerDelegate::CreateLambda([this] { CurrentWeapon->SetActorHiddenInGame(false); }),
								   0.25, false);
}

void APlayerCharacter::OnWeaponDeactivated(ABaseWeapon* Weapon)
{
	// todo: Does nothing
}

void APlayerCharacter::PlayWeaponMontage(UAnimMontage* Montage)
{
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
	}
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
	}
}

void APlayerCharacter::AddRecoil(float RecoilStrength)
{
	AddControllerPitchInput(-RecoilStrength);
}

void APlayerCharacter::OnEnemyHit(int Points)
{
	GainScore(Points);
}

void APlayerCharacter::OnShotFired()
{
	if (PlayerUIRef)
	{
		PlayerUIRef->BP_ShotFired();
	}
}
