// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthUser.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHealthUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CODZOMBIESCLONE_API IHealthUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnHealthUIUpdate(const int& CurrentHealth, const int& MaxHealth) = 0;
	virtual void OnDeath() = 0;
};
