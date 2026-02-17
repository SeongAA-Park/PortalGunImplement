// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPortalable.h"
#include "Cb00_SpawnGlassroomPTWall.generated.h"

UCLASS()
class PORTALGUNIMPLEMENT_API ACb00_SpawnGlassroomPTWall : public AActor, public IIPortalable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* func_detail_8256;
	
public:	
	// Sets default values for this actor's properties
	ACb00_SpawnGlassroomPTWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	virtual AActor* GetPortalSurfaceActor_Implementation() override;

};
