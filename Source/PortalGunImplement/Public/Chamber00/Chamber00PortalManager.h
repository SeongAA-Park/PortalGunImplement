// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chamber00PortalManager.generated.h"

class ACustomPortal;
class UBoxComponent;
class USceneComponent;
class ACb00_InternalDoor;


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

	/** 트리거 Overlap 콜백 - 엘리베이터 앞 포탈 제거 collisionbox */
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
	//버튼에 연결되어 있지 않고, 월드 수준으로 관리되는 Door 존재 - 월드에 스폰되어 있는 인스턴스 참조하기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Chamber00|InternalDoor")
	TObjectPtr<ACb00_InternalDoor> Cb00ManagedDoor;
	
	/** ====== World Managed Door Close Trigger ====== */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Chamber00|InternalDoor", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> WorldManagedDoorCloseTrigger;
	
	/** 한번만 반응하도록 가드 */
	bool bWorldDoorCloseTriggered = false;
	
	/** 트리거 Overlap 콜백 - 매니저에서 관리하는 문 닫힘용 collisionbox */
	UFUNCTION()
	void OnWorldManagedDoorCloseTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
private:
	UFUNCTION()
	void OpenManagedDoorDelayed();
	
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
