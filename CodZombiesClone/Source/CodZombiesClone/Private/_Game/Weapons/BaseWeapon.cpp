// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Weapons/BaseWeapon.h"

#include "CodZombiesClone.h"
#include "_Game/Data/FWeaponDataTableRow.h"
#include "_Game/Weapons/WeaponUser.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Fps Mesh
	FpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FpsMesh"));
	FpsMesh->SetupAttachment(RootComponent);
	FpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FpsMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FpsMesh->bOnlyOwnerSee = true;

	// Tps Mesh
	TpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TpsMesh"));
	TpsMesh->SetupAttachment(RootComponent);
	TpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	TpsMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	TpsMesh->bOwnerNoSee = true;
}

void ABaseWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Cache data from the data table
	if (FWeaponDataTableRow* data = WeaponData.GetRow<FWeaponDataTableRow>(FString()))
	{
		FpsMesh->SetSkeletalMesh(data->FpsWeaponMesh.LoadSynchronous());
		TpsMesh->SetSkeletalMesh(data->FpsWeaponMesh.LoadSynchronous());

		FiringMontage = data->FiringMontage.LoadSynchronous();
		FirstPersonAnimInstanceClass = data->FirstPersonAnimInstanceClass;
		ThirdPersonAnimInstanceClass = data->ThirdPersonAnimInstanceClass;

		MagazineSize = data->MagazineSize;
		RecoilStrength = data->RecoilStrength;
		FireRate = data->FireRate;
	}
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Ensure weapon destroys with owner
	GetOwner()->OnDestroyed.AddDynamic(this, &ABaseWeapon::OnOwnerDestroyed);

	// Always starts fully reloaded
	CurrentAmmo = MagazineSize;

	// Cache Weapon User & Attach to user
	WeaponUser = Cast<IWeaponUser>(GetOwner());
	WeaponUser->AttachWeapon(this);

	// Update UI
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
}

void ABaseWeapon::StartFiring()
{
	// todo: need to implement firing logic here, but for now this is just a placeholder to test the weapon system.
	if (bFireCooldownActive) return;
	// Reload handling - Pretty shit but it works for now.
	if (CurrentAmmo <= 0)
	{
		CurrentAmmo = MagazineSize;
		WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
		return;
	}

	FVector startLocation = FVector::ZeroVector;
	FVector direction = FVector::ZeroVector;
	FVector endLocation = FVector::ZeroVector;

	WeaponUser->GetTargetAimLocation(startLocation, direction);
	endLocation = startLocation + (direction * 100.0f);

	// Visual stuff
	WeaponUser->PlayWeaponFireMontage(FiringMontage);

	CurrentAmmo--;
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);

	// Recoi
	WeaponUser->AddRecoil(RecoilStrength);

	bFireCooldownActive = true;
	GetWorld()->GetTimerManager().SetTimer(FireCooldownTimer,
	                                       FTimerDelegate::CreateLambda([this] { bFireCooldownActive = false; }),
	                                       FireRate, false);

	// Debug stuff
	// UE_LOG(Khaled, Display, TEXT("Firing weapon! Aim Location: %s"), *startLocation.ToString());
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 2.0f, 0, 1.0f);
}

void ABaseWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}
