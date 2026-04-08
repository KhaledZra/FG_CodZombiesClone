// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Weapons/BaseWeapon.h"

#include "_Game/Data/FWeaponDataTableRow.h"
#include "_Game/Weapons/WeaponUser.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Fps Mesh
	FpsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FpsMesh"));
	FpsMesh->SetupAttachment(RootComponent);
	FpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FpsMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FpsMesh->bOnlyOwnerSee = true;

	// Tps Mesh
	TpsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TpsMesh"));
	TpsMesh->SetupAttachment(RootComponent);
	TpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	TpsMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	TpsMesh->bOwnerNoSee = true;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Ensure weapon destroys with owner
	GetOwner()->OnDestroyed.AddDynamic(this, &ABaseWeapon::OnOwnerDestroyed);

	// Always starts fully reloaded
	CurrentAmmo = GetMagazineSize();
	
	// Cache Weapon User & Attach to user
	WeaponUser = Cast<IWeaponUser>(GetOwner());
	WeaponUser->AttachWeapon(this);
}

int ABaseWeapon::GetMagazineSize() const
{
	if (FWeaponDataTableRow* data = WeaponData.GetRow<FWeaponDataTableRow>(FString()))
	{
		return data->CurrentMagazineSize;
	}

	return 0;
}

void ABaseWeapon::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}

void ABaseWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (FWeaponDataTableRow* data = WeaponData.GetRow<FWeaponDataTableRow>(FString()))
	{
		FpsMesh->SetStaticMesh(data->FpsWeaponMesh.LoadSynchronous());
		TpsMesh->SetStaticMesh(data->FpsWeaponMesh.LoadSynchronous());
	}
}
