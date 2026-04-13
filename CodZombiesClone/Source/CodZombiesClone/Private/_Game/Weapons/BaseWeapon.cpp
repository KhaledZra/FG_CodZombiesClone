// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Weapons/BaseWeapon.h"

#include "CodZombiesClone.h"
#include "Kismet/KismetSystemLibrary.h"
#include "_Game/Components/HealthComponent.h"
#include "_Game/Data/FWeaponDataTableRow.h"
#include "_Game/GameModes/FpsGameMode.h"
#include "_Game/Interfaces/WeaponUser.h"


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
		ReloadMontage = data->ReloadMontage.LoadSynchronous();
		EquippedMontage = data->EquippedMontage.LoadSynchronous();
		DryFireMontage = data->DryFireMontage.LoadSynchronous(); // Not used cause it looks wonky.
		
		FirstPersonAnimInstanceClass = data->FirstPersonAnimInstanceClass;
		ThirdPersonAnimInstanceClass = data->ThirdPersonAnimInstanceClass;

		MagazineSize = data->MagazineSize;
		RecoilStrength = data->RecoilStrength;
		FireRate = data->FireRate;
		BulletRange = data->BulletRange;
		GunDamage = data->GunDamage;
		bAutoFire = data->bAutoFire;
		ReloadLength = data->ReloadLength;
	}
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	// Ensure weapon destroys with owner
	GetOwner()->OnDestroyed.AddDynamic(this, &ABaseWeapon::OnOwnerDestroyed);

	// Always starts fully reloaded
	CurrentAmmo = MagazineSize;

	// Cache Weapon User & Attach to user
	WeaponUser = Cast<IWeaponUser>(GetOwner());
	WeaponUser->AttachWeapon(this);

	// Update UI
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
	
	WeaponUser->OnWeaponActivated(this);
}

void ABaseWeapon::StartFiring()
{
	if (bFireCooldownActive) return;
	if (bIsReloading) return;
	if (CurrentAmmo <= 0)
	{
		Reload();
		return;
	}

	if (!FMath::IsNearlyEqual(FireRate, 0.0f))
	{
		bFireCooldownActive = true;
		GetWorld()->GetTimerManager().SetTimer(FireCooldownTimer,
		                                       FTimerDelegate::CreateLambda([this] { bFireCooldownActive = false; }),
		                                       FireRate, false);
	}

	Fire();
}

void ABaseWeapon::StopFiring()
{
	if (bAutoFire) // Timer is only used when u auto fire
	{
		UE_LOG(Khaled, Log, TEXT("Auto Fire Stopped"));
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
	}
}

void ABaseWeapon::Reload()
{
	if (CurrentAmmo == MagazineSize) return;
	if (bIsReloading) return;
	
	UE_LOG(Khaled, Log, TEXT("Reload Started"));
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer,
										   this, &ABaseWeapon::OnReloadComplete,
										   ReloadLength, false);
	
	WeaponUser->PlayWeaponMontage(ReloadMontage);
}

void ABaseWeapon::ActivateWeapon()
{
	WeaponUser->PlayWeaponMontage(EquippedMontage);
}

void ABaseWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}

void ABaseWeapon::Fire()
{
	if (bIsReloading) return;
	if (CurrentAmmo <= 0) return;
	
	FVector startLocation = FVector::ZeroVector;
	FVector direction = FVector::ZeroVector;
	FVector endLocation = FVector::ZeroVector;

	WeaponUser->GetTargetAimLocation(startLocation, direction);
	endLocation = startLocation + (direction * BulletRange);

	// Visual stuff
	WeaponUser->PlayWeaponMontage(FiringMontage);

	CurrentAmmo--;
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);

	// Recoil
	WeaponUser->AddRecoil(RecoilStrength);

	// Do the linetrace stuff
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldStatic);
	bool bTraceComplex = true;
	TArray<AActor*> ActorsToIgnore;
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
	FHitResult OutHit;
	bool bIgnoreSelf = true;

	// Get player actors
	if (const AFpsGameMode* gm = GetWorld()->GetAuthGameMode<AFpsGameMode>())
	{
		ActorsToIgnore = gm->PlayerActors;
	}
	else ActorsToIgnore.Add(GetOwner());

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), startLocation, endLocation, TraceChannel, bTraceComplex,
	                                      ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf);

	if (OutHit.bBlockingHit && OutHit.GetActor() != nullptr)
	{
		if (UHealthComponent* HealthComp = OutHit.GetActor()->FindComponentByClass<UHealthComponent>())
		{
			// UE_LOG(Khaled, Display, TEXT("Hit Actor: %s"), *OutHit.GetActor()->GetName());
			HealthComp->TakeDamage(GunDamage);
		}
	}

	// If it's auto set a timer to keep firing
	if (bAutoFire)
	{
		UE_LOG(Khaled, Log, TEXT("Auto Fire Triggered"));
		float fireRate = FMath::IsNearlyEqual(FireRate, 0.0f) ? 0.01f : FireRate;
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer,
		                                       this, &ABaseWeapon::Fire,
		                                       fireRate, false);
	}
}

void ABaseWeapon::OnReloadComplete()
{
	CurrentAmmo = MagazineSize;
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
	bIsReloading = false;
}
