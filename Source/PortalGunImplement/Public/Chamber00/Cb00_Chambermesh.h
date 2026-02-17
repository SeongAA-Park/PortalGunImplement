// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPortalable.h"
#include "Cb00_Chambermesh.generated.h"

UCLASS()
class PORTALGUNIMPLEMENT_API ACb00_Chambermesh : public AActor, public IIPortalable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* worldspawn_1;
	
public:	
	// Sets default values for this actor's properties
	ACb00_Chambermesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	// --- IPortalable 인터페이스 구현 ---
	// BlueprintNativeEvent로 선언했으므로 _Implementation을 오버라이드합니다.
	virtual AActor* GetPortalSurfaceActor_Implementation() override;

};
