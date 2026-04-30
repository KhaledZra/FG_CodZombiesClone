// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/_Game/Characters/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_Game/Components/HealthComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(5.0f, 5.89f, -5.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetEnableFirstPersonFieldOfView(true);
	FirstPersonCameraComponent->SetEnableFirstPersonScale(true);
	FirstPersonCameraComponent->SetFirstPersonFieldOfView(90.0f);
	FirstPersonCameraComponent->SetFirstPersonScale(0.9f);

	// Create the health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	GetMesh()->SetRelativeLocationAndRotation(FVector(-20.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 400.0f / 2.0f;
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 3072.0f;
	GetCharacterMovement()->BrakingFriction = 1.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 200.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
}

void ABaseCharacter::DoAim(float Yaw, float Pitch)
{
	if (!GetController()) return;
	
	
	AddControllerYawInput(Yaw * (LookSensitivityScale * GetWorld()->GetDeltaSeconds()));
	AddControllerPitchInput(Pitch * (LookSensitivityScale * GetWorld()->GetDeltaSeconds()));
}

void ABaseCharacter::DoMove(float Right, float Forward)
{
	if (!GetController()) return;
	
	AddMovementInput(GetActorRightVector(), Right);
	AddMovementInput(GetActorForwardVector(), Forward);
}

void ABaseCharacter::DoJumpStart()
{
	Jump();
}

void ABaseCharacter::DoJumpEnd()
{
	StopJumping();
}

void ABaseCharacter::OnDeath()
{
	Destroy();
}

void ABaseCharacter::OnHealthUIUpdate(const int& CurrentHealth, const int& MaxHealth)
{
	// Empty, overriden in other classes
}

void ABaseCharacter::OnHurt()
{
	// Stub
}

