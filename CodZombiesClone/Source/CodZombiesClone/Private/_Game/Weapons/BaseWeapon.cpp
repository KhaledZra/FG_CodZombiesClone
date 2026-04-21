// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Weapons/BaseWeapon.h"

#include "CodZombiesClone.h"
#include "Kismet/KismetMathLibrary.h"
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

		FpsMesh->SetAnimInstanceClass(data->WeaponAnimInstanceClass);
		TpsMesh->SetAnimInstanceClass(data->WeaponAnimInstanceClass);

		FiringMontage = data->FiringMontage.LoadSynchronous();
		ReloadMontage = data->ReloadMontage.LoadSynchronous();
		EquippedMontage = data->EquippedMontage.LoadSynchronous();
		DryFireMontage = data->DryFireMontage.LoadSynchronous();

		WeaponReloadMontage = data->WeaponReloadMontage.LoadSynchronous();
		WeaponFireMontage = data->WeaponFireMontage.LoadSynchronous();

		FirstPersonAnimInstanceClass = data->FirstPersonAnimInstanceClass;
		ThirdPersonAnimInstanceClass = data->ThirdPersonAnimInstanceClass;

		MagazineSize = data->MagazineSize;
		RecoilStrength = data->RecoilStrength;
		FireRate = data->FireRate;
		BulletRange = data->BulletRange;
		GunDamage = data->GunDamage;
		bAutoFire = data->bAutoFire;
		bShotgunSpread = data->bShotgunSpread;
		SpreadCount = data->SpreadCount;
		MaxSpreadDegree = data->MaxSpreadDegree;
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

	WeaponUser->OnWeaponActivated(this);

	UE_LOG(Khaled, Log, TEXT("Weapon BeginPlay"));
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
	BP_PlayAnimMontage(WeaponReloadMontage);
}

void ABaseWeapon::ActivateWeapon()
{
	// Update UI
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
	WeaponUser->PlayWeaponMontage(EquippedMontage);
}

void ABaseWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}

void ABaseWeapon::Fire()
{
	if (bIsReloading) return;
	if (CurrentAmmo <= 0)
	{
		WeaponUser->PlayWeaponMontage(DryFireMontage);
		return;
	}

	// Visual stuff
	WeaponUser->PlayWeaponMontage(FiringMontage);
	BP_PlayAnimMontage(WeaponFireMontage);

	CurrentAmmo--;
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
	WeaponUser->OnShotFired();

	// Recoil
	WeaponUser->AddRecoil(RecoilStrength);

	// Do the linetrace stuff
	FVector startLocation = FVector::ZeroVector;
	FVector direction = FVector::ZeroVector;
	WeaponUser->GetTargetAimLocation(startLocation, direction);
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	// Get player actors
	TArray<AActor*> ActorsToIgnore;
	if (const AFpsGameMode* gm = GetWorld()->GetAuthGameMode<AFpsGameMode>())
	{
		ActorsToIgnore = gm->PlayerActors;
	}
	else ActorsToIgnore.Add(GetOwner());

	if (bShotgunSpread)
	{
		for (int i = 0; i < SpreadCount; i++)
		{
			FireBulletRay(startLocation, direction, TraceChannel, ActorsToIgnore, true);
		}
	}
	else
	{
		FireBulletRay(startLocation, direction, TraceChannel, ActorsToIgnore, true);
	}

	// If it's auto set a timer to keep firing
	if (bAutoFire)
	{
		// UE_LOG(Khaled, Log, TEXT("Auto Fire Triggered"));
		float fireRate = FMath::IsNearlyEqual(FireRate, 0.0f) ? 0.01f : FireRate;
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer,
		                                       this, &ABaseWeapon::Fire,
		                                       fireRate, false);
	}
}

void ABaseWeapon::FireBulletRay(const FVector& StartLocation, const FVector& Direction,
                                const ETraceTypeQuery& TraceChannel, const TArray<AActor*>& ActorsToIgnore,
                                bool bDrawDebug)
{
	FVector endLocation = FVector::ZeroVector;
	FVector direction = Direction;
	
	if (bShotgunSpread)
	{
		direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Direction, MaxSpreadDegree);
	}
	
	endLocation = StartLocation + (direction * BulletRange);
	
	
	bool bTraceComplex = true;
	EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	FHitResult OutHit;
	bool bIgnoreSelf = true;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, endLocation, TraceChannel, bTraceComplex,
	                                      ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf);

	if (OutHit.bBlockingHit == false || OutHit.GetActor() == nullptr) return;
	UHealthComponent* HealthComp = OutHit.GetActor()->FindComponentByClass<UHealthComponent>();
	if (HealthComp == nullptr || HealthComp->IsDead()) return;

	bool bIsDead = false;
	HealthComp->TakeDamage(GunDamage, OutHit.BoneName.ToString(), bIsDead);

	// Todo: hardcoded points, change to a manager that register points better
	// Normal hit equals 10 points
	// Kill equals 50 points
	// Headshot kill equals 100 points

	bool bHeadshotKill = bIsDead && OutHit.BoneName.ToString().Equals("head", ESearchCase::IgnoreCase);
	int pointsGained = bIsDead ? (bHeadshotKill ? 100 : 50) : 10;
	WeaponUser->OnEnemyHit(pointsGained);
}

void ABaseWeapon::OnReloadComplete()
{
	CurrentAmmo = MagazineSize;
	WeaponUser->UpdateWeaponHud(CurrentAmmo, MagazineSize);
	bIsReloading = false;
}
