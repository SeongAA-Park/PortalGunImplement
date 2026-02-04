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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
	TObjectPtr<class USceneCaptureComponent2D> PortalCamera;
	
	//(beginplay로부터 전역변수화) PortalCamera가 찍은 장면(Scene Capture)을 픽셀 이미지로 렌더링해서 저장해두는 버퍼
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Portal")
	TObjectPtr<class UTextureRenderTarget2D> PortalRT;

	//(beginplay로부터 전역변수화) PortalCamera에 지정해줄 Material - 런타임에 파라미터만 바꿔 머터리얼 변경 가능
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Portal")
	TObjectPtr<class UMaterialInstanceDynamic> PortalMID;

	
	//3.포탈 종류 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int32 PortalID;
	
	//4.연결된 반대편 포탈 참조
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	TObjectPtr<ACustomPortal> LinkedPortal;
	
protected:
	void UpdatePortalView();
	
	void UpdatePortalView2(); //포탈 예제를 기반으로 작성해본 코드
};
