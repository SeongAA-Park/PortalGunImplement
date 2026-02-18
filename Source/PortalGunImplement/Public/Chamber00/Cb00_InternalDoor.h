// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cb00_InternalDoor.generated.h"



UCLASS()
class PORTALGUNIMPLEMENT_API ACb00_InternalDoor : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> Doorframe;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> Rtdoor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> Lftdoor;
	
public:	
	// Sets default values for this actor's properties
	ACb00_InternalDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintCallable, Category="Door")
	void DoorOpen();

	UFUNCTION(BlueprintCallable, Category="Door")
	void DoorClose();

private:
	// 닫힌 상태 위치 저장
	FVector RtdoorClosedRelLoc;
	FVector LftdoorClosedRelLoc;

	// 0(닫힘) ~ 1(열림)
	float CurrentAlpha = 0.f;
	float TargetAlpha = 0.f;

	UPROPERTY(EditAnywhere, Category="Door|Motion")
	float SlideDistance = 13.f;

	// 문이 열리고 닫히는 속도 (Alpha per second가 아니라 cm/s 느낌으로 만들기 위해 아래에 사용)
	UPROPERTY(EditAnywhere, Category="Door|Motion", meta=(ClampMin="1.0"))
	float SlideSpeedCmPerSec = 5.f;

	// 좌우로 벌어지는 축 (기본 Y축)
	UPROPERTY(EditAnywhere, Category="Door|Motion")
	FVector SlideAxis = FVector(0.f, 1.f, 0.f);

	// 타이머 주기(업데이트 빈도)
	UPROPERTY(EditAnywhere, Category="Door|Motion", meta=(ClampMin="0.005", ClampMax="0.1"))
	float UpdateInterval = 0.016f; // 약 60Hz

	FTimerHandle DoorMotionTimer;

private:
	UFUNCTION()
	void UpdateDoorMotion();

	void StartDoorTimerIfNeeded();
	void ApplyDoorTransform(float Alpha);
	
};
