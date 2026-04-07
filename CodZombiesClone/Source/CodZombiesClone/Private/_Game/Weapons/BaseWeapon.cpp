// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Weapons/BaseWeapon.h"

#include "_Game/Data/FWeaponDataTableRow.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Fps Mesh
	FpsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FpsMesh"));
	FpsMesh->SetupAttachment(RootComponent);
	FpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Tps Mesh
	FpsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TpsMesh"));
	FpsMesh->SetupAttachment(RootComponent);
	FpsMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

int ABaseWeapon::GetMagazineSize() const
{
	if (FWeaponDataTableRow* data = WeaponData.GetRow<FWeaponDataTableRow>(FString()))
	{
		return data->CurrentMagazineSize;
	}

	return 0;
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

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}
