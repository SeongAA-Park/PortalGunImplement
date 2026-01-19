// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomPortal.generated.h"

UCLASS()
class PORTALGUNIMPLEMENT_API ACustomPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomPortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	//1.포탈 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UStaticMeshComponent> PortalMesh;
	
	//2.반대편을 찍어서 보여줄 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class USceneCaptureComponent2D> PortalCamera;
	
	//3.포탈 종류 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int32 PortalID;
	
	//4.연결된 반대편 포탈 참조
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	TObjectPtr<ACustomPortal> LinkedPortal;

};
