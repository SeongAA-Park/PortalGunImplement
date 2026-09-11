// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/CustomPortalBase.h"
#include "CustomPortal.generated.h"

/**
 * 
 */
UCLASS()
class PORTALGUNIMPLEMENT_API ACustomPortal : public ACustomPortalBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void CheckPlayerTeleport();
	
};
