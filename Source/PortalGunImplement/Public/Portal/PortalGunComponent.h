// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PortalGunComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTALGUNIMPLEMENT_API UPortalGunComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPortalGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	/** 캐릭터의 발사 신호를 수신 (0: 블루, 1: 오렌지) */
	UFUNCTION()
	void HandlePortalShot(int32 PortalColorIndex);
	
	/** 실제 포탈 발사 로직 */
	void ExecutePortalTrace(int32 ColorIndex);

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
