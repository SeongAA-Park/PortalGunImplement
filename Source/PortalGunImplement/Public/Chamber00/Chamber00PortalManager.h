// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chamber00PortalManager.generated.h"

class ACustomPortal;
class UBoxComponent;
class USceneComponent;

UCLASS()
class PORTALGUNIMPLEMENT_API AChamber00PortalManager : public AActor
{
	GENERATED_BODY()
	
protected:
	/** ====== Spawn Points ====== */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chamber00|Portals", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chamber00|Portals", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> BlueSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chamber00|Portals", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> OrangeSpawnPoint;

	/** ====== Elevator Cleanup Trigger ====== */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chamber00|Cleanup", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> ElevatorCleanupTrigger;

	/** ====== Config ====== */

	/** 스폰할 포탈 클래스(당신이 PortalGun에서 쓰던 CustomPortal BP/Class 지정) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chamber00|Config")
	TSubclassOf<ACustomPortal> PortalClass;

	/** 게임 시작 후 몇 초 뒤 포탈을 생성할지 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chamber00|Config", meta=(ClampMin="0.0", Units="s"))
	float PortalSpawnDelay = 20.0f;

	/** 포탈이 붙을 벽(수동 지정). 반드시 Portalable surface 액터로 지정해둘 것 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chamber00|Config")
	TObjectPtr<AActor> BlueAttachedWall;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chamber00|Config")
	TObjectPtr<AActor> OrangeAttachedWall;

	/** ====== Runtime ====== */
	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> BluePortal;

	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> OrangePortal;

	FTimerHandle SpawnTimerHandle;
	
	/** 지연 후 호출: 포탈 2개 생성 + 링크 */
	UFUNCTION()
	void SpawnPortalPair();

	/** 엘리베이터 트리거 진입 시 호출: 포탈 제거 */
	UFUNCTION()
	void RemovePortalPair();

	/** 트리거 Overlap 콜백 */
	UFUNCTION()
	void OnCleanupTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** 내부 유틸 */
	bool IsPortalPairSpawned() const;
	void DestroyPortalSafe(TObjectPtr<ACustomPortal>& PortalPtr);
public:	
	// Sets default values for this actor's properties
	AChamber00PortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
