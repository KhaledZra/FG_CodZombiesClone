// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "FpsPlayerCameraManager.generated.h"

/**
 *  Basic First Person camera manager.
 *  Limits min/max look pitch.
 */
UCLASS()
class CODZOMBIESCLONE_API AFpsPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	AFpsPlayerCameraManager();
};
