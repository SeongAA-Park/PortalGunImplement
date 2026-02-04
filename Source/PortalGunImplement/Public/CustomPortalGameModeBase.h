// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomPortalGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTALGUNIMPLEMENT_API ACustomPortalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PortalUI")
	TSubclassOf<UUserWidget> BasicCrossHairClass;
	
	
};
