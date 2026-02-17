// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/Turret.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 0. 루트 생성
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
	
	// 1. 메시 설정
	TurretSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretSkeletalMesh"));
	TurretSkeletalMesh->SetupAttachment(RootComponent);

	// 2. 총구 위치 설정 (메시의 자식으로 붙임)
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(TurretSkeletalMesh);
	MuzzleLocation->SetRelativeLocation(FVector(40.f, 0.f, 50.f)); // 적절히 조정 필요

	// 3. 감지 범위 설정
	// DetectionRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionRange"));
	// DetectionRange->SetupAttachment(TurretSkeletalMesh);
	// DetectionRange->SetSphereRadius(600.0f); // 감지 반경 600
	// DetectionRange->SetCollisionProfileName(TEXT("Trigger")); // 트리거 설정

	// 변수 초기화
	//TargetActor = nullptr;

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	// 오버랩 이벤트 바인딩
	// DetectionRange->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlapBegin);
	// DetectionRange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnOverlapEnd);
	
	CachedPlayer = UGameplayStatics::GetPlayerPawn(this, 0);

	SightCosThreshold = FMath::Cos(
		FMath::DegreesToRadians(SightAngle * 0.5f)
	);
}

void ATurret::CheckSight(float DT)
{
	// 플레이어가 없으면 그때만 다시 찾는다
	if (!CachedPlayer)
	{
		CachedPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
		if (!CachedPlayer)
			return;
	}

	const FVector TurretLocation = GetActorLocation();
	const FVector PlayerLocation = CachedPlayer->GetActorLocation();

	const FVector ToPlayer = PlayerLocation - TurretLocation;

	// 1️⃣ 거리 체크 (제곱 거리 사용)
	const float DistanceSq = ToPlayer.SizeSquared();
	if (DistanceSq > SightRange * SightRange)
		return;

	// 2️⃣ 시야각 체크
	const FVector Direction = ToPlayer.GetSafeNormal();
	const float Dot = FVector::DotProduct(GetActorForwardVector(), Direction);

	if (Dot < SightCosThreshold)
		return;

	// 3️⃣ LineTrace로 가시성 확인
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		TurretLocation,
		PlayerLocation,
		ECC_Visibility,
		Params
	);
	
	// 디버그라인 그리기
	FColor LineColor = bHit ? FColor::Red : FColor::Green;

	DrawDebugLine(
		GetWorld(),
		TurretLocation,
		PlayerLocation,
		LineColor,
		false,      // persistent (false면 한 프레임만)
		0.1f,       // 지속 시간
		0,
		1.5f        // 선 두께
	);

	if (bHit && Hit.GetActor() == CachedPlayer)
	{
		AimToPlayer(DT,TurretLocation, PlayerLocation);
		// 여기서 상태 전환 또는 공격 함수 호출
		FireProjectile();
	}
}

void ATurret::AimToPlayer(float DT, FVector StartLocation, FVector TargetLocation)
{
	// 타겟이 유효할 때만 회전 로직 실행
	// 1. 타겟을 향하는 회전값 계산
	//FVector StartLocation = GetActorLocation();
	//FVector TargetLocation = CachedPlayer->GetActorLocation();
        
	// FindLookAtRotation: A에서 B를 바라보는 회전값 반환
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	// 2. Z축(Yaw)만 회전하도록 제한 (Pitch, Roll은 고정)
	FRotator TargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);

	// 3. 부드러운 회전 (Interpolation)
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DT, RotationSpeed);
        
	SetActorRotation(NewRotation);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckSight(DeltaTime);
	
}

// void ATurret::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
// 	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// 들어온 액터가 플레이어(Character)인지 확인
// 	if (OtherActor && (OtherActor != this) && OtherActor->IsA(ACharacter::StaticClass()))
// 	{
// 		TargetActor = OtherActor;
//
// 		// 발사 타이머 시작 (Loop: true)
// 		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret::FireProjectile, FireRate, true);
// 	}
// }

// void ATurret::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
// 	int32 OtherBodyIndex)
// {
// 	// 나간 액터가 현재 타겟이라면
// 	if (OtherActor && (OtherActor == TargetActor))
// 	{
// 		TargetActor = nullptr;
//
// 		// 발사 타이머 정지
// 		GetWorldTimerManager().ClearTimer(FireTimerHandle);
// 	}
// }

void ATurret::FireProjectile()
{
	if (!ProjectileClass) return;

	// MuzzleLocation의 위치와 회전값 가져오기
	FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
	FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// 투사체 스폰
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
    
	// (선택사항) 여기서 발사 소리나 이펙트를 재생할 수 있습니다.
}

