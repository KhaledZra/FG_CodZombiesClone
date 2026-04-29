// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"
#include "_Game/Interfaces/HealthUser.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// It's fine GetOwner() does not use IHealthUser, since not everyone might wanna actually have extra attached logic
	HealthUser = Cast<IHealthUser>(GetOwner());
}


void UHealthComponent::UpdateHealthGeneration()
{
	Heal(HealthGenerationHeal);

	if (IsFullHealth())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenHandle);
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (bHealthRegenerationActive)
	{
		GetWorld()->GetTimerManager().SetTimer(HealthRegenHandle, this, &UHealthComponent::UpdateHealthGeneration, 0.05f, true, HealthGenerationDelay);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::SetMaxHealth(const int& Health, const bool& bForceHeal)
{
	MaxHealth = Health;
	CurrentHealth = bForceHeal ? MaxHealth : CurrentHealth;
}

void UHealthComponent::Heal(const int& HealAmount)
{
	CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
	if (HealthUser) HealthUser->OnHealthUIUpdate(CurrentHealth, MaxHealth);
}

void UHealthComponent::FullHeal()
{
	CurrentHealth = MaxHealth;
	if (HealthUser) HealthUser->OnHealthUIUpdate(CurrentHealth, MaxHealth);
}

void UHealthComponent::TakeDamage(const int& DamageAmount, const FString& BodyPartName, const FVector& HitLocation,
                                  const FRotator& RotationNormal, bool& bOutIsDead)
{
	bOutIsDead = false;

	if (HitSplatVfx)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitSplatVfx,
			HitLocation,
			RotationNormal + FRotator(-90.f, 0.f, 0.f)
		);
	}
	
	if (bIsGodMode) return;

	// Hardcoded ngl
	int finalDamageAmount = BodyPartName.Equals("head", ESearchCase::IgnoreCase) ? DamageAmount * 2 : DamageAmount;

	CurrentHealth = FMath::Max(CurrentHealth - finalDamageAmount, 0);
	if (HealthUser)
	{
		HealthUser->OnHealthUIUpdate(CurrentHealth, MaxHealth);
		HealthUser->OnHurt();
	}

	if (CurrentHealth <= 0)
	{
		Die();
		bOutIsDead = true;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenHandle);
		
		if (bHealthRegenerationActive)
		{
			GetWorld()->GetTimerManager().SetTimer(HealthRegenHandle, this, &UHealthComponent::UpdateHealthGeneration, 0.05f, true, HealthGenerationDelay);
		}
	}
}

void UHealthComponent::Die()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenHandle);

	if (HealthUser) HealthUser->OnDeath();
	else GetOwner()->Destroy();
}

bool UHealthComponent::IsDead()
{
	if (bIsGodMode) return false;
	return CurrentHealth <= 0;
}

bool UHealthComponent::IsFullHealth()
{
	return CurrentHealth >= MaxHealth;
}
