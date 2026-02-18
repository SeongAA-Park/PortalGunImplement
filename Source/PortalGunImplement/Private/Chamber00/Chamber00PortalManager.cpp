// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Chamber00PortalManager.h"
#include "Components/BoxComponent.h"
#include "Portal/CustomPortal.h"

void AChamber00PortalManager::SpawnPortalPair()
{
	// 이미 있다면 정리 후 재생성
	RemovePortalPair();

	if (!PortalClass)
	{
		UE_LOG(LogTemp, Error, TEXT("[Chamber00PortalManager] PortalClass is not set."));
		return;
	}

	if (!IsValid(BlueSpawnPoint) || !IsValid(OrangeSpawnPoint))
	{
		UE_LOG(LogTemp, Error, TEXT("[Chamber00PortalManager] SpawnPoints are invalid."));
		return;
	}

	// AttachedWall은 수동 지정한다고 하셨으니, 없으면 경고만 띄우고 진행은 막을지 정책 선택
	// 여기서는 "없으면 스폰은 하되 SetAttachedWall은 생략" 대신,
	// 충돌/통과 로직이 AttachedWall에 의존하는 걸 고려해 스폰 자체를 막습니다.
	if (!IsValid(BlueAttachedWall) || !IsValid(OrangeAttachedWall))
	{
		UE_LOG(LogTemp, Error, TEXT("[Chamber00PortalManager] AttachedWall is not set (Blue or Orange)."));
		return;
	}

	// 스폰 트랜스폼은 SpawnPoint의 월드 트랜스폼 그대로 사용
	const FTransform BlueXform = BlueSpawnPoint->GetComponentTransform();
	const FTransform OrangeXform = OrangeSpawnPoint->GetComponentTransform();

	// 포탈 스폰 파라미터
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ====== Blue Portal ======
	BluePortal = GetWorld()->SpawnActorDeferred<ACustomPortal>(
		PortalClass,
		BlueXform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!IsValid(BluePortal))
	{
		UE_LOG(LogTemp, Error, TEXT("[Chamber00PortalManager] Failed to spawn BluePortal."));
		return;
	}

	// 당신 코드 기반: 0=Blue, 1=Orange
	BluePortal->PortalID = 0;
	BluePortal->SetAttachedWall(BlueAttachedWall);
	BluePortal->FinishSpawning(BlueXform);

	// ====== Orange Portal ======
	OrangePortal = GetWorld()->SpawnActorDeferred<ACustomPortal>(
		PortalClass,
		OrangeXform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!IsValid(OrangePortal))
	{
		UE_LOG(LogTemp, Error, TEXT("[Chamber00PortalManager] Failed to spawn OrangePortal."));
		DestroyPortalSafe(BluePortal);
		return;
	}

	OrangePortal->PortalID = 1;
	OrangePortal->SetAttachedWall(OrangeAttachedWall);
	OrangePortal->FinishSpawning(OrangeXform);

	// ====== Link ======
	BluePortal->LinkedPortal = OrangePortal;
	OrangePortal->LinkedPortal = BluePortal;

	UE_LOG(LogTemp, Log, TEXT("[Chamber00PortalManager] Portal pair spawned and linked."));
	
	
}

void AChamber00PortalManager::RemovePortalPair()
{
	DestroyPortalSafe(BluePortal);
	DestroyPortalSafe(OrangePortal);
}

void AChamber00PortalManager::OnCleanupTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA<APawn>())
	{
		if (IsValid(BluePortal) || IsValid(OrangePortal))
		{
			UE_LOG(LogTemp, Log, TEXT("[Chamber00PortalManager] Cleanup trigger entered. Removing portals."));
			RemovePortalPair();
		}
	}
}

bool AChamber00PortalManager::IsPortalPairSpawned() const
{
	return IsValid(BluePortal) && IsValid(OrangePortal);
}

void AChamber00PortalManager::DestroyPortalSafe(TObjectPtr<ACustomPortal>& PortalPtr)
{
	if (IsValid(PortalPtr))
	{
		PortalPtr->Destroy();
	}
	PortalPtr = nullptr;
}

// Sets default values
AChamber00PortalManager::AChamber00PortalManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	BlueSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BlueSpawnPoint"));
	BlueSpawnPoint->SetupAttachment(Root);

	OrangeSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("OrangeSpawnPoint"));
	OrangeSpawnPoint->SetupAttachment(Root);

	ElevatorCleanupTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ElevatorCleanupTrigger"));
	ElevatorCleanupTrigger->SetupAttachment(Root);

	// 트리거 기본 세팅(필요시 BP에서 크기/위치 조정)
	ElevatorCleanupTrigger->SetBoxExtent(FVector(120.f, 120.f, 200.f));
	ElevatorCleanupTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ElevatorCleanupTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ElevatorCleanupTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ElevatorCleanupTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AChamber00PortalManager::BeginPlay()
{
	Super::BeginPlay();
	
	// Cleanup trigger 바인딩
	if (ensure(ElevatorCleanupTrigger))
	{
		ElevatorCleanupTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AChamber00PortalManager::OnCleanupTriggerBeginOverlap
		);
	}

	// 지연 스폰 예약
	if (PortalSpawnDelay <= 0.0f)
	{
		SpawnPortalPair();
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AChamber00PortalManager::SpawnPortalPair,
			PortalSpawnDelay,
			false
		);
	}
	
}

// Called every frame
// void AChamber00PortalManager::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

