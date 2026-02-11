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
	//포탈 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UStaticMeshComponent> PortalMesh;
	
	//반대편을 찍어서 보여줄 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Portal")
	TObjectPtr<class USceneCaptureComponent2D> PortalCamera;
	
	//box collision : 플레이어 detect 및 포탈이 붙어있는 plane wall의 플레이어 및 물체 통과를 위한 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UBoxComponent> DetectBoxCollision; 
	
	//(beginplay로부터 전역변수화) PortalCamera가 찍은 장면(Scene Capture)을 픽셀 이미지로 렌더링해서 저장해두는 버퍼
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Portal")
	TObjectPtr<class UTextureRenderTarget2D> PortalRT;

	//(beginplay로부터 전역변수화) PortalCamera에 지정해줄 Material - 런타임에 파라미터만 바꿔 머터리얼 변경 가능
	UPROPERTY(Transient, VisibleInstanceOnly, Category="Portal")
	TObjectPtr<class UMaterialInstanceDynamic> PortalMID;
	
	//02.09 렌더링로직 수정중 - 추가
	// 원본 머티리얼 (NewCP_Material) - 블루프린트에서 Create Dynamic Material Instance의 Parent에 매터리얼을 넣은것과 같이 필요
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UMaterialInterface> NewCP_Material;

	
	//3.포탈 종류 구분
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	int32 PortalID;
	
	//4.연결된 반대편 포탈 참조
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	TObjectPtr<ACustomPortal> LinkedPortal;
	
protected:
	// 안정성을 위해 약한 포인터(Weak Pointer)를 사용합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Portal")
	TWeakObjectPtr<AActor> AttachedWall;
	
protected:
	void UpdatePortalView3(); //02.09 새로작성
	
	// 오버랩 이벤트 함수 (반드시 UFUNCTION이어야 합니다)
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	// 벽 정보를 설정하기 위한 public 함수
	void SetAttachedWall(AActor* InWall) { AttachedWall = InWall; }
};
