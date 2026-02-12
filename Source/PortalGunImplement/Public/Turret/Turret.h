// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class PORTALGUNIMPLEMENT_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleLocation; // 총알이 나가는 위치

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* DetectionRange; // 감지 범위

	// 설정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	float RotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	TSubclassOf<class AActor> ProjectileClass; // 발사할 투사체 클래스

private:
	// 타겟 관리
	AActor* TargetActor;
	FTimerHandle FireTimerHandle;

	// 감지 이벤트 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 발사 로직
	void FireProjectile();
	
};
