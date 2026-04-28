// Fill out your copyright notice in the Description page of Project Settings.


#include "_Game/Characters/ZombieCharacter.h"

#include "CodZombiesClone.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "_Game/AIController/EnemyAIController.h"
#include "_Game/Characters/PlayerCharacter.h"
#include "_Game/Components/HealthComponent.h"
#include "_Game/Managers/ZombieWaveManager.h"


// Sets default values
AZombieCharacter::AZombieCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set 'bUseAccelerationForPaths = true' in bp
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	// makes em turn smoother
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	GetMesh()->SetCollisionProfileName("Enemy");
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AIControllerClass = EnemyAIController;

	AttackRange = 25.0f;
	AttackSphereRadius = 25.0f;
	AttackDamage = 10.0f;
}

void AZombieCharacter::AttackForward()
{
	// Play Anim Montage
	if (!AttackMontages.IsEmpty())
	{
		BP_PlayAnimMontage(AttackMontages[FMath::RandRange(0, AttackMontages.Num() - 1)]);
	}

	// Hardcoded
	FVector attackLocation = GetActorLocation() + (GetActorForwardVector() * AttackRange);
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), attackLocation, AttackSphereRadius, 12, FLinearColor::Red, 1.0f,
	                                      1.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Visibility));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Camera));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	UClass* ActorClassFilter = APlayerCharacter::StaticClass();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), attackLocation, AttackSphereRadius, ObjectTypes,
	                                          ActorClassFilter, ActorsToIgnore, OutActors);

	if (OutActors.IsEmpty()) return;

	for (AActor* hitActor : OutActors)
	{
		UE_LOG(Khaled, Log, TEXT("Hit: %s"), *hitActor->GetName());
		if (UHealthComponent* hc = hitActor->FindComponentByClass<UHealthComponent>())
		{
			bool bIsDead = false; // not really cared for rn.
			hc->TakeDamage(AttackDamage, "", hitActor->GetActorLocation(), hitActor->GetActorRotation(), bIsDead);
		}
	}
}

void AZombieCharacter::SetupZombie(FZombieStats Stats)
{
	AttackDamage = Stats.Damage;
	GetCharacterMovement()->MaxWalkSpeed = Stats.Speed;
	HealthComponent->SetMaxHealth(Stats.Health, true);
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
}

void AZombieCharacter::OnDeath()
{
	if (AEnemyAIController* aic = Cast<AEnemyAIController>(GetController()))
	{
		aic->StopAILogic();
	}

	// Precall this then clear it since it will broadcast again
	OnDestroyed.Broadcast(this);
	OnDestroyed.Clear();

	// Enable ragdoll
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);

	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	// Bugs out players hand controller
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

	GetWorld()->GetTimerManager().SetTimer(OnDeathTimerHandle,
	                                       FTimerDelegate::CreateLambda([this]
	                                       {
		                                       Destroy();
	                                       }), 5.0f, false);
}

void AZombieCharacter::OnHurt()
{
	if (HurtAnimMontages.IsEmpty()) return;

	// todo: could be enhanced by playing based on hitlocation or by using PhysicalAnimations
	BP_PlayAnimMontage(HurtAnimMontages[FMath::RandRange(0, HurtAnimMontages.Num() - 1)]);
}
