// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Components/HealthComponent.h"

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


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = StartingMaxHealth;
	CurrentHealth = MaxHealth;
	if (HealthUser) HealthUser->OnHealthUIUpdate(CurrentHealth, MaxHealth);
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

void UHealthComponent::TakeDamage(const int& DamageAmount)
{
	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0);
	if (HealthUser) HealthUser->OnHealthUIUpdate(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0) Die();
}

void UHealthComponent::Die()
{
	if (HealthUser) HealthUser->OnDeath();
	else GetOwner()->Destroy();
}

