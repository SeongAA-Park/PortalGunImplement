// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chamber06PortalManager.generated.h"

class UBoxComponent;
class ACb00_InternalDoor;

UCLASS()
class PORTALGUNIMPLEMENT_API AChamber06PortalManager : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> Root;
	
	// 레벨에 배치된 문 인스턴스 2개를 지정 (BP 인스턴스)
	UPROPERTY(EditInstanceOnly, Category="Cb06|Door")
	TObjectPtr<ACb00_InternalDoor> Door1;

	UPROPERTY(EditInstanceOnly, Category="Cb06|Door")
	TObjectPtr<ACb00_InternalDoor> Door2;

	// 문을 “통과한 뒤” 마주치는 트리거 2개 (레벨에 배치된 BoxCollision 컴포넌트 참조)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> Door1CloseTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Trigger", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> Door2CloseTrigger;
	
private:
	bool bDoor1ClosedOnce = false;
	bool bDoor2ClosedOnce = false;

	void DisableTrigger(UBoxComponent* Trigger) const;

	void TryCloseDoorOnce(ACb00_InternalDoor* Door, UBoxComponent* Trigger, bool& bClosedOnce, AActor* OtherActor);

	UFUNCTION()
	void OnDoor1CloseTriggerOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnDoor2CloseTriggerOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
public:	
	// Sets default values for this actor's properties
	AChamber06PortalManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
