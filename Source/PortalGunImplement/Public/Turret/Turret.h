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
	
	void CheckSight(float DT);
	
	void AimToPlayer(float DTFVector ,FVector StartLocation, FVector TargetLocation);
	
	void FireProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 컴포넌트 선언
	
	// 터렛의 메쉬를 스켈레탈메쉬로 변경
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* TurretSkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence* IdleAnim; //기본상태애님
	
	// 애니메이션 시퀀스 할당용 변수 (에디터에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence* OpenAnim; // 날개 펴기
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UAnimSequence* CloseAnim; // 대기 상태로 복귀
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleLocation; // 총알이 나가는 위치

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// class USphereComponent* DetectionRange; // 감지 범위 >> 컴포넌트를 사용할지, Tick에서 수학 계산을 사용할지 고민중...
	
	TObjectPtr<APawn> CachedPlayer;

	UPROPERTY(EditAnywhere, Category="Sight")
	float SightRange = 1500.f;

	UPROPERTY(EditAnywhere, Category="Sight")
	float SightAngle = 60.f;

	float SightCosThreshold;
	
	// 설정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	float RotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	float FireRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Settings")
	TSubclassOf<class AActor> ProjectileClass; // 발사할 투사체 클래스

private:
	// 타겟 관리
	//AActor* TargetActor;
	//FTimerHandle FireTimerHandle;

	// 감지 이벤트 함수
	// UFUNCTION()
	// void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	// UFUNCTION()
	// void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
