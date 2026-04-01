// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPortalable.h"
#include "Cb06_FirstSpawnOrPTWall.generated.h"

UCLASS()
class PORTALGUNIMPLEMENT_API ACb06_FirstSpawnOrPTWall : public AActor, public IIPortalable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* func_detail_16394;
	
public:	
	// Sets default values for this actor's properties
	ACb06_FirstSpawnOrPTWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	virtual AActor* GetPortalSurfaceActor_Implementation() override;

};
