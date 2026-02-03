// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalGunPIckup.generated.h"

// 포탈건 픽업의 타입을 정의합니다.
UENUM(BlueprintType)
enum class EPortalPickupType : uint8
{
	Blue    UMETA(DisplayName = "Blue Portal Gun"),
	Orange  UMETA(DisplayName = "Orange Portal Gun")
};

UCLASS()
class PORTALGUNIMPLEMENT_API APortalGunPIckup : public AActor
{
	GENERATED_BODY()
	
	/** 픽업 충돌 범위 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollision;

	/** 픽업될 아이템의 외형 (직육면체나 구체 등으로 설정 가능) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PickupMesh;
	
protected:
	/** 이 픽업이 어떤 색상의 포탈건 권한을 줄지 결정합니다. */
	UPROPERTY(EditAnywhere, Category = "Pickup Settings")
	EPortalPickupType PickupType;
	
public:	
	// Sets default values for this actor's properties
	APortalGunPIckup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** 충돌 시 호출될 함수 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
