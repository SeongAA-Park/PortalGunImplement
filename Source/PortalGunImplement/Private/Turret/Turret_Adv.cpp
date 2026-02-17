// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret/Turret_Adv.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimSequence.h"

// Sets default values
ATurret_Adv::ATurret_Adv()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	// SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PivotRoot"));
	// RootComponent = SceneRoot;

	// Skeletal Mesh
	TurretSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretSkeletalMesh"));
	RootComponent = TurretSkeletalMesh;
	
	// eye component
	EyePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EyePoint"));
	EyePoint->SetupAttachment(TurretSkeletalMesh);
	EyePoint->SetRelativeLocation(FVector(0.0f, 10.0f, 90.0));

	// Detection Sphere (1차 후보 감지)
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->SetupAttachment(TurretSkeletalMesh);
	DetectionSphere->SetSphereRadius(DetectionRadius);

	// 충돌 설정은 프로젝트 상황에 따라 다를 수 있어,
	// 기본은 QueryOnly + Overlap로 잡아둠
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void ATurret_Adv::BeginPlay()
{
	Super::BeginPlay();
	
	// Sphere radius가 에디터에서 바뀌었다면 반영
	DetectionSphere->SetSphereRadius(DetectionRadius);

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret_Adv::OnDetectionBegin);
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret_Adv::OnDetectionEnd);

	// 시야각 threshold
	SightCosThreshold = FMath::Cos(FMath::DegreesToRadians(SightAngle * 0.5f));

	// 초기 회전 저장(감지 종료 시 복귀)
	InitialRotation = GetActorRotation();

	// AnimBP 없이 단일 노드 재생 모드
	if (TurretSkeletalMesh)
	{
		TurretSkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(
		// 		-1,                 // Key: -1이면 매번 새로운 메시지 출력, 특정 숫자면 해당 메시지 갱신
		// 		5.0f,               // Duration: 메시지가 화면에 머무는 시간 (초)
		// 		FColor::Cyan,       // DisplayColor: 텍스트 색상
		// 		TEXT("Setted Animation Single Node") // DebugMessage: 출력할 문자열
		// 	);
		// }
	}
	SetState(ETurretState::Idle);
}

// Called every frame
void ATurret_Adv::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugSphere(GetWorld(), DetectionSphere->GetComponentLocation(), DetectionSphere->GetScaledSphereRadius(),
		16, FColor::Yellow, false, 0.05f);
	
	// 2차 확정 (후보가 있을 때만)
	const bool bSeesPlayer = ConfirmSight(DeltaTime);

	// 보이는 동안은 Aim 유지
	if (bSeesPlayer && CachedPlayer)
	{
		AimToPlayer(DeltaTime, GetActorLocation(), CachedPlayer->GetActorLocation());
	}
	else
	{
		// 감지 종료/대기 중에는 초기 방향으로 복귀(Closing/Idle에서 자연스럽게 돌아가게)
		if (State == ETurretState::Closing || State == ETurretState::Idle)
		{
			ReturnToInitialRotation(DeltaTime);
		}
	}
}

void ATurret_Adv::OnDetectionBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                 // Key: -1이면 매번 새로운 메시지 출력, 특정 숫자면 해당 메시지 갱신
				5.0f,               // Duration: 메시지가 화면에 머무는 시간 (초)
				FColor::Red,       // DisplayColor: 텍스트 색상
				TEXT("Player is not casting on detectionsphere") // DebugMessage: 출력할 문자열
			);
		}
		return;
	}

	// 후보 지정
	CandidatePlayer = Pawn;
	
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Overlap Begin: %s"), *GetNameSafe(OtherActor));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
	}
}

void ATurret_Adv::OnDetectionEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;

	// 후보가 나가면 후보/타겟 해제
	if (CandidatePlayer.Get() == Pawn)
	{
		CandidatePlayer = nullptr;
		
		if (GEngine)
		{
			FString Msg = FString::Printf(TEXT("Overlap End: %s"), *GetNameSafe(OtherActor));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Msg);
		}
	}

	if (CachedPlayer.Get() == Pawn)
	{
		CachedPlayer = nullptr;

		// 공격/오픈 중이었다면 종료 동작으로
		if (State == ETurretState::Attacking || State == ETurretState::Opening)
		{
			SetState(ETurretState::Closing);
		}
	}
}

bool ATurret_Adv::ConfirmSight(float DeltaTime)
{
	if (!CandidatePlayer)
	{
		// 후보가 없으면 최종 타겟도 없음
		CachedPlayer = nullptr;
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                 // Key: -1이면 매번 새로운 메시지 출력, 특정 숫자면 해당 메시지 갱신
				5.0f,               // Duration: 메시지가 화면에 머무는 시간 (초)
				FColor::Red,       // DisplayColor: 텍스트 색상
				TEXT("There is no CandidatePlayer") // DebugMessage: 출력할 문자열
			);
		}

		// 공격 중이었다면 닫기로
		if (State == ETurretState::Attacking || State == ETurretState::Opening)
		{
			SetState(ETurretState::Closing);
		}
		return false;
	}

	const FVector TurretLocation = this->EyePoint->GetComponentLocation();
	UE_LOG(LogTemp,Warning,TEXT("Turret Loc : %f, %f, %f"),TurretLocation.X,TurretLocation.Y,TurretLocation.Z);
	const FVector PlayerLocation = CandidatePlayer->GetActorLocation();
	UE_LOG(LogTemp,Warning,TEXT("Player Loc : %f, %f, %f"),PlayerLocation.X,PlayerLocation.Y,PlayerLocation.Z);

	const FVector ToPlayer = (PlayerLocation - TurretLocation);
	const FVector Dir = ToPlayer.GetSafeNormal();

	// 시야각 Dot 체크
	const float Dot = FVector::DotProduct(EyePoint->GetRightVector(), Dir);
	if (Dot < SightCosThreshold)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                 // Key: -1이면 매번 새로운 메시지 출력, 특정 숫자면 해당 메시지 갱신
				5.0f,               // Duration: 메시지가 화면에 머무는 시간 (초)
				FColor::Cyan,       // DisplayColor: 텍스트 색상
				TEXT("Player is Not on sight") // DebugMessage: 출력할 문자열
			);
		}
		// 각도 밖이면 "안 보임" 처리
		CachedPlayer = nullptr;

		if (State == ETurretState::Attacking || State == ETurretState::Opening)
		{
			SetState(ETurretState::Closing);
		}
		return false;
	}

	// LineTrace로 가시성 확인
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

	const bool bVisible = (bHit && Hit.GetActor() == CandidatePlayer);

	if (bDrawSightDebug)
	{
		const FColor LineColor = bVisible ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), TurretLocation, PlayerLocation, LineColor, false, 0.05f, 0, 1.2f);
	}
	
	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("Visible: %s"),
			bVisible ? TEXT("TRUE") : TEXT("FALSE"));
		GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::White, Msg);
	}
	
	if (GEngine)
	{
		FString HitMsg = FString::Printf(TEXT("bHit=%d HitActor=%s HitComp=%s"),
			bHit ? 1 : 0,
			*GetNameSafe(Hit.GetActor()),
			*GetNameSafe(Hit.GetComponent()));
		GEngine->AddOnScreenDebugMessage(3, 0.5f, FColor::Magenta, HitMsg);
	}


	if (!bVisible)
	{
		CachedPlayer = nullptr;

		if (State == ETurretState::Attacking || State == ETurretState::Opening)
		{
			SetState(ETurretState::Closing);
		}
		return false;
	}

	// 여기까지 오면 "확정 시야 내 + 가시성 OK"
	CachedPlayer = CandidatePlayer;

	// 상태 전환: Idle/Closing이면 Opening으로
	if (State == ETurretState::Idle || State == ETurretState::Closing)
	{
		SetState(ETurretState::Opening);
	}

	return true;
}

void ATurret_Adv::AimToPlayer(float DeltaTime, const FVector& StartLocation, const FVector& TargetLocation)
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	// Yaw만 회전
	const FRotator TargetRotation(0.f, LookAtRotation.Yaw, 0.f);

	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	SetActorRotation(NewRotation);
}

void ATurret_Adv::ReturnToInitialRotation(float DeltaTime)
{
	const FRotator TargetRotation(0.f, InitialRotation.Yaw, 0.f);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	SetActorRotation(NewRotation);
}

void ATurret_Adv::SetState(ETurretState NewState)
{
	if (State == NewState) return;

	// 상태 종료 처리
	if (State == ETurretState::Attacking)
	{
		StopFire();
	}

	// 애님 타이머 정리
	GetWorldTimerManager().ClearTimer(OpenAnimTimerHandle);
	GetWorldTimerManager().ClearTimer(CloseAnimTimerHandle);

	State = NewState;

	switch (State)
	{
	case ETurretState::Idle:
		PlayIdle();
		break;

	case ETurretState::Opening:
		PlayOpen();
		break;

	case ETurretState::Attacking:
		StartFire();
		break;

	case ETurretState::Closing:
		PlayClose();
		break;
	}
}

void ATurret_Adv::PlayIdle()
{
	if (TurretSkeletalMesh && IdleAnim)
	{
		TurretSkeletalMesh->PlayAnimation(IdleAnim, true);
	}
}

void ATurret_Adv::PlayOpen()
{
	if (!TurretSkeletalMesh)
	{
		SetState(ETurretState::Attacking);
		return;
	}

	if (!OpenAnim)
	{
		// 오픈 애님이 없으면 즉시 공격
		SetState(ETurretState::Attacking);
		return;
	}

	TurretSkeletalMesh->PlayAnimation(OpenAnim, false);

	const float Len = OpenAnim->GetPlayLength();
	GetWorldTimerManager().SetTimer(OpenAnimTimerHandle, this, &ATurret_Adv::OnOpenFinished, Len, false);
}

void ATurret_Adv::PlayClose()
{
	// 닫는 동안에도 초기 방향 복귀는 Tick에서 수행(Closing/Idle일 때)
	if (!TurretSkeletalMesh)
	{
		SetState(ETurretState::Idle);
		return;
	}

	if (!CloseAnim)
	{
		SetState(ETurretState::Idle);
		return;
	}

	TurretSkeletalMesh->PlayAnimation(CloseAnim, false);

	const float Len = CloseAnim->GetPlayLength();
	GetWorldTimerManager().SetTimer(CloseAnimTimerHandle, this, &ATurret_Adv::OnCloseFinished, Len, false);
	
}

void ATurret_Adv::OnOpenFinished()
{
	// 오픈 끝났는데, 여전히 플레이어가 보이면 공격, 아니면 닫기
	if (State != ETurretState::Opening) return;

	if (CachedPlayer)
	{
		SetState(ETurretState::Attacking);
	}
	else
	{
		SetState(ETurretState::Closing);
	}
}

void ATurret_Adv::OnCloseFinished()
{
	if (State != ETurretState::Closing) return;

	// 닫기 끝나면 Idle
	SetState(ETurretState::Idle);
}

void ATurret_Adv::StartFire()
{
	if (GetWorldTimerManager().IsTimerActive(FireTimerHandle))
		return;

	// 즉시 1회 발사 후 반복
	FireProjectile();

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATurret_Adv::FireProjectile, FireRate, true);
}

void ATurret_Adv::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ATurret_Adv::FireProjectile()
{
	if (State != ETurretState::Attacking) return;
	if (!ProjectileClass || !TurretSkeletalMesh) return;
	if (!CachedPlayer) return; // 타겟이 사라졌으면 발사 중지 성격

	// 소켓 존재 확인
	const bool bHasLeft = TurretSkeletalMesh->DoesSocketExist(LeftMuzzleSocketName);
	const bool bHasRight = TurretSkeletalMesh->DoesSocketExist(RightMuzzleSocketName);

	if (!bHasLeft && !bHasRight) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	// 왼쪽/오른쪽 동시 발사
	if (bHasLeft)
	{
		const FTransform TM = TurretSkeletalMesh->GetSocketTransform(LeftMuzzleSocketName, RTS_World);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, TM.GetLocation(), TM.Rotator(), SpawnParams);
	}

	if (bHasRight)
	{
		const FTransform TM = TurretSkeletalMesh->GetSocketTransform(RightMuzzleSocketName, RTS_World);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, TM.GetLocation(), TM.Rotator(), SpawnParams);
	}
}

