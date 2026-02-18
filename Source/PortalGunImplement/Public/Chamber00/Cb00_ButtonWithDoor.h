// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cb00_ButtonWithDoor.generated.h"

class UBoxComponent;
class ACb00_InternalDoor;

UCLASS()
class PORTALGUNIMPLEMENT_API ACb00_ButtonWithDoor : public AActor
{
	GENERATED_BODY()
	
protected:
	//컴포넌트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> ButtonTop;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> ButtonBase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> ButtonTrigger;
	
public:
	
	//연결되어 있는 InternalDoor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LinkedDoor")
	TObjectPtr<ACb00_InternalDoor> LinkedDoor;
	
private:
	// ButtonTop 기본 위치
	FVector ButtonTopInitialRelLoc;

	// 눌림 깊이 (Z축 -10)
	UPROPERTY(EditAnywhere, Category="Button|Motion")
	float PressDepth = 10.f;

	// 현재 누르고 있는 액터들(중복/여러 개체 대응)
	TSet<TWeakObjectPtr<AActor>> PressingActors;

	bool bIsPressed = false;
	
public:	
	// Sets default values for this actor's properties
	ACb00_ButtonWithDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// ===== Overlap Events =====
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	bool IsValidPresser(AActor* OtherActor, UPrimitiveComponent* OtherComp) const;
	void UpdatePressedState();
	void SetButtonVisualPressed(bool bPressed);

};
