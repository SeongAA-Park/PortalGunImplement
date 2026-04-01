// Fill out your copyright notice in the Description page of Project Settings.
#include "Chamber06/Chamber06PortalManager.h"
#include "Chamber00/Cb00_InternalDoor.h"
#include "Components/BoxComponent.h"




// Sets default values
AChamber06PortalManager::AChamber06PortalManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// =========================
	// Door1 Trigger 생성
	// =========================
	Door1CloseTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Door1CloseTrigger"));
	Door1CloseTrigger->SetupAttachment(Root);

	Door1CloseTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Door1CloseTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Door1CloseTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Door1CloseTrigger->SetBoxExtent(FVector(100.f, 100.f, 200.f)); // 기본 크기
	Door1CloseTrigger->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // BP에서 조정 가능

	// =========================
	// Door2 Trigger 생성
	// =========================
	Door2CloseTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Door2CloseTrigger"));
	Door2CloseTrigger->SetupAttachment(Root);

	Door2CloseTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Door2CloseTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Door2CloseTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Door2CloseTrigger->SetBoxExtent(FVector(100.f, 100.f, 200.f));
	Door2CloseTrigger->SetRelativeLocation(FVector(300.f, 0.f, 0.f)); // 기본 위치

}

void AChamber06PortalManager::DisableTrigger(UBoxComponent* Trigger) const
{
	if (!IsValid(Trigger)) return;

	Trigger->SetGenerateOverlapEvents(false);
	Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Trigger->SetHiddenInGame(true); // 원하면 사용(디버그용)
}

void AChamber06PortalManager::TryCloseDoorOnce(ACb00_InternalDoor* Door, UBoxComponent* Trigger, bool& bClosedOnce,
	AActor* OtherActor)
{
	if (bClosedOnce) return;
	if (!IsValid(Door) || !IsValid(Trigger) || !IsValid(OtherActor)) return;

	// "플레이어만" 닫히게: Pawn 기준(원하면 APortalGunShooterCharacter로 Cast해서 더 엄격히 가능)
	if (!OtherActor->IsA<APawn>()) return;

	// 1회 닫기
	Door->DoorClose();
	bClosedOnce = true;

	// 다시는 작동하지 않게 트리거 비활성화
	DisableTrigger(Trigger);
}

void AChamber06PortalManager::OnDoor1CloseTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TryCloseDoorOnce(Door1, Door1CloseTrigger, bDoor1ClosedOnce, OtherActor);
}

void AChamber06PortalManager::OnDoor2CloseTriggerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TryCloseDoorOnce(Door2, Door2CloseTrigger, bDoor2ClosedOnce, OtherActor);
}



// Called when the game starts or when spawned
void AChamber06PortalManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 1) 시작 시 두 문 모두 열림
	if (IsValid(Door1)) Door1->DoorOpen();
	if (IsValid(Door2)) Door2->DoorOpen();

	// 2) 트리거 Overlap 바인딩
	if (IsValid(Door1CloseTrigger))
	{
		Door1CloseTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AChamber06PortalManager::OnDoor1CloseTriggerOverlap);
	}

	if (IsValid(Door2CloseTrigger))
	{
		Door2CloseTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AChamber06PortalManager::OnDoor2CloseTriggerOverlap);
	}
	
}

// Called every frame
// void AChamber06PortalManager::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

