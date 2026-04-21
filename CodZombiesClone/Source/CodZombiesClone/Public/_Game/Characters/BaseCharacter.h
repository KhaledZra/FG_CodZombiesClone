// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "_Game/Interfaces/HealthUser.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 *  The main base character
 */
UCLASS(abstract)
class CODZOMBIESCLONE_API ABaseCharacter : public ACharacter, public IHealthUser
{
	GENERATED_BODY()
	
protected:
	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	// IHealthUser interface
	virtual void OnDeath() override;
	virtual void OnHealthUIUpdate(const int& CurrentHealth, const int& MaxHealth) override;
	virtual void OnHurt() override;
};
