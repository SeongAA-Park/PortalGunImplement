// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_InternalDoor.h"

// Sets default values
ACb00_InternalDoor::ACb00_InternalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Doorframe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorframe"));
	Doorframe->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Doorframeasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/props_door_01_frame_wide_reference.props_door_01_frame_wide_reference"));
	if (Doorframeasset.Succeeded())
	{
		Doorframe->SetStaticMesh(Doorframeasset.Object);
	}
	
	Rtdoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rtdoor"));
	Rtdoor->SetupAttachment(Doorframe);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Rtdoorasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/props_door_01_rtdoor_reference_003.props_door_01_rtdoor_reference_003"));
	if (Rtdoorasset.Succeeded())
	{
		Rtdoor->SetStaticMesh(Rtdoorasset.Object);
	}
	
	Lftdoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lftdoor"));
	Lftdoor->SetupAttachment(Doorframe);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Lftdoorasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/props_door_01_lftdoor_reference_003.props_door_01_lftdoor_reference_003"));
	if (Lftdoorasset.Succeeded())
	{
		Lftdoor->SetStaticMesh(Lftdoorasset.Object);
	}
	
	Doorframe->SetRelativeScale3D(FVector(160.f,160.f,160.f));
	//Rtdoor->SetRelativeScale3D(FVector(160.f,160.f,160.f));
	//Lftdoor->SetRelativeScale3D(FVector(160.f,160.f,160.f));
	
	Rtdoor->SetRelativeLocation(FVector(0.f,0.f,0.f));
	Lftdoor->SetRelativeLocation(FVector(0.f,0.f,0.f));
	

}

// Called when the game starts or when spawned
void ACb00_InternalDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Rtdoor) RtdoorClosedRelLoc = Rtdoor->GetRelativeLocation();
	if (Lftdoor) LftdoorClosedRelLoc = Lftdoor->GetRelativeLocation();

	SlideAxis = SlideAxis.GetSafeNormal();
	if (SlideAxis.IsNearlyZero())
	{
		SlideAxis = FVector(0.f, 1.f, 0.f);
	}

	// 시작 상태는 닫힘
	CurrentAlpha = 0.f;
	TargetAlpha = 0.f;
	ApplyDoorTransform(CurrentAlpha);
	
	// DoorOpen();
	//
	// FTimerHandle Temp;
	// GetWorldTimerManager().SetTimer(
	// 	Temp,
	// 	this,
	// 	&ACb00_InternalDoor::DoorClose,
	// 	2.0f,
	// 	false
	// );
	
}

void ACb00_InternalDoor::DoorOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("DoorOpen called: %s (Target was %.3f)"), *GetName(), TargetAlpha);
	
	TargetAlpha = 1.f;
	StartDoorTimerIfNeeded();
	
	UE_LOG(LogTemp, Warning, TEXT("DoorOpen set TargetAlpha=%.3f"), TargetAlpha);
}

void ACb00_InternalDoor::DoorClose()
{
	UE_LOG(LogTemp, Warning, TEXT("DoorClose called: %s (Target was %.3f)"), *GetName(), TargetAlpha);

	TargetAlpha = 0.f;
	StartDoorTimerIfNeeded();
}

void ACb00_InternalDoor::UpdateDoorMotion()
{
	// 안전 체크
	if (!Rtdoor || !Lftdoor || !GetWorld())
	{
		UE_LOG(LogTemp,Error,TEXT("There is no Rtdoor or Lftdoor or GetWorld"))
		GetWorldTimerManager().ClearTimer(DoorMotionTimer);
		return;
	}
	
	// UpdateInterval 기준으로 "이번 틱에서 움직일 Alpha 변화량" 계산
	// cm/s -> alpha/s로 바꾸려면 Distance로 나눔 (한쪽 문 기준이 아니라 전체 슬라이드 거리 기준)
	const float AlphaSpeedPerSec = SlideSpeedCmPerSec / FMath::Max(SlideDistance, 1.f);
	const float Step = AlphaSpeedPerSec * UpdateInterval;
	
	UE_LOG(LogTemp,Warning,TEXT("TargetAlpha During UpdateDoorMotion : %f"), TargetAlpha);

	// 목표로 수렴
	if (CurrentAlpha < TargetAlpha)
	{
		CurrentAlpha = FMath::Min(CurrentAlpha + Step, TargetAlpha);
	}
	else if (CurrentAlpha > TargetAlpha)
	{
		CurrentAlpha = FMath::Max(CurrentAlpha - Step, TargetAlpha);
	}

	ApplyDoorTransform(CurrentAlpha);
	
	// 👇 여기 추가
	UE_LOG(LogTemp, Warning, TEXT("Door %s Alpha=%.3f RtLoc=%s Target=%.3f"),
		*GetName(),
		CurrentAlpha,
		*Rtdoor->GetRelativeLocation().ToString(),
		TargetAlpha
	);
	

	// 목표 도달 시 타이머 정지
	const float Eps = 0.0001f;
	if (FMath::Abs(CurrentAlpha - TargetAlpha) <= Eps)
	{
		CurrentAlpha = TargetAlpha;
		ApplyDoorTransform(CurrentAlpha);
		GetWorldTimerManager().ClearTimer(DoorMotionTimer);
	}
	
}

void ACb00_InternalDoor::StartDoorTimerIfNeeded()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("No World in StartDoorTimerIfNeeded"));
		return;
	}
	
	const bool bActive = GetWorldTimerManager().IsTimerActive(DoorMotionTimer);
	UE_LOG(LogTemp, Warning, TEXT("StartDoorTimerIfNeeded: active=%d"), bActive);
	
	if (!bActive)
	{
		GetWorldTimerManager().SetTimer(
			DoorMotionTimer,
			this,
			&ACb00_InternalDoor::UpdateDoorMotion,
			UpdateInterval,
			true
		);
	
		UE_LOG(LogTemp, Warning, TEXT("Timer set. active now=%d"), GetWorldTimerManager().IsTimerActive(DoorMotionTimer));
		
		UE_LOG(LogTemp, Warning, TEXT("TargetAlpha During StartDoorTimerIfNeeded : %f"),TargetAlpha);
	}
	
	// 이미 타이머가 돌고 있으면 그대로(목표만 바뀜)
	if (!GetWorldTimerManager().IsTimerActive(DoorMotionTimer))
	{
		GetWorldTimerManager().SetTimer(
			DoorMotionTimer,
			this,
			&ACb00_InternalDoor::UpdateDoorMotion,
			UpdateInterval,
			true
		);
	}
	
	// if (!GetWorld()) return;
	//
	// const float Interval = FMath::Max(UpdateInterval, 0.005f);
	//
	// // 같은 Handle이면 SetTimer가 갱신됩니다. (기존 타이머가 있어도 안전)
	// GetWorldTimerManager().SetTimer(
	// 	DoorMotionTimer,
	// 	this,
	// 	&ACb00_InternalDoor::UpdateDoorMotion,
	// 	Interval,
	// 	true
	// );
}

void ACb00_InternalDoor::ApplyDoorTransform(float Alpha)
{
	const FVector Offset = SlideAxis * SlideDistance * Alpha;

	Rtdoor->SetRelativeLocation(RtdoorClosedRelLoc + Offset);
	Lftdoor->SetRelativeLocation(LftdoorClosedRelLoc - Offset);
}

// Called every frame
// void ACb00_InternalDoor::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

