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
	// 스폰할 포탈 클래스 (에디터에서 ACustomPortal 블루프린트를 할당 필요)
	UPROPERTY(EditAnywhere, Category = "Portal")
	TSubclassOf<class ACustomPortal> PortalClass;

	// 이미 생성된 포탈들을 관리하기 위한 변수 (0: 블루, 1: 오렌지)
	// 현재 월드에 생성된 포탈들의 주소를 기억하고 새로 쓸 대 기존 것을 지우는 로직 담당을 위해
	//생성된 파란색 포탈의 주소를 기억합니다.
	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> BluePortal;
	
	//생성된 주황색 포탈의 주소를 기억합니다.
	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> OrangePortal; 

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
