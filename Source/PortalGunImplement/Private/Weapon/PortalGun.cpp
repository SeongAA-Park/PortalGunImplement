// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PortalGun.h"
#include "Portal/CustomPortal.h"
#include "Portal/PortalGunShooterCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "SNegativeActionButton.h"
#include "Camera/CameraComponent.h"
#include "Runtime/CoreUObject/Tests/UObject/PropertyStateTrackingTest.h"

// Sets default values
APortalGun::APortalGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 1. 공통 루트 컴포넌트 생성 (SceneComponent)
	// 메쉬들을 독립적으로 조절하기 위해 빈 씬 컴포넌트를 루트로 삼습니다.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 2. 1인칭 무기 메쉬 설정
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);
    
	// 템플릿 설정 반영: 물리 충돌 제거 및 1인칭 렌더링 최적화
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FirstPersonMesh->bOnlyOwnerSee = true; // 소유자에게만 보임
	FirstPersonMesh->CastShadow = false;   // 1인칭 총기 그림자가 화면을 가리는 것 방지 (Added)

	// 3. 3인칭 무기 메쉬 설정
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);

	// 템플릿 설정 반영: 물리 충돌 제거 및 월드 공간 렌더링
	ThirdPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	ThirdPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	ThirdPersonMesh->bOwnerNoSee = true;   // 소유자에게는 보이지 않음 (1인칭과 겹침 방지)
}

// Called when the game starts or when spawned
void APortalGun::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* MyOwner = GetOwner())
	{
		// 주인 파괴 시 나도 파괴되도록 바인딩
		//EX : MyOwner->OnDestroyed.AddDynamic(this, &APortalGun::OnOwnerDestroyed);
        
		// 캐릭터 캐싱
		PortalHoldingPlayer = Cast<APortalGunShooterCharacter>(MyOwner);
        
		if (PortalHoldingPlayer)
		{
			// 캐릭터의 메쉬에 이 총을 부착 (이전에 만든 함수 활용)
			// 주의: 포탈건 캐릭터가 인터페이스를 안 쓴다면 직접 함수를 호출해야 함
			//PortalHoldingPlayer->AttachWeaponMeshes(this); 이미 PortalGunCharacter의 BeginPlay에서 사용중
		}
	}
	
}

void APortalGun::HandlePortalShot(int32 PortalColorIndex)
{
	// 캐릭터의 조준 함수를 기반으로 실제 포탈 배치 수행
	ExecutePortalTrace(PortalColorIndex);
}

void APortalGun::ExecutePortalTrace(int32 ColorIndex)
{
	if (!PortalHoldingPlayer || !PortalClass)
	{
		return; 
	}
	
	//수정 : 캐릭터의 시선 정중앙의 타겟 정보 가져오기
	FHitResult PlayerHitResult = PortalHoldingPlayer->GetWeaponTargetLocation();
	
	if (PlayerHitResult.bBlockingHit)
	{
		// 포탈 소환 트랜스폼 계산
		// 벽면에 겹쳐서 깜빡이는 Z-Fighting을 방지하기 위해 Normal 방향으로 0.1cm 띄움
		FVector SpawnLocation = PlayerHitResult.ImpactPoint + (PlayerHitResult.ImpactNormal * 0.1f);
        
		// 벽의 Normal 방향을 포탈의 앞방향(X축)으로 설정하여 벽에 평평하게 붙임
		FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(PlayerHitResult.ImpactNormal);

		// 4. 기존 동일 색상 포탈 제거 (교체 로직)
		if (ColorIndex == 0 && BluePortal) BluePortal->Destroy();
		else if (ColorIndex == 1 && OrangePortal) OrangePortal->Destroy();

		// 5. 액터 스폰 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = PortalHoldingPlayer;
    	
		// [추가!] 충돌 조건과 상관없이 항상 스폰하도록 설정
		//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 실제 포탈 인스턴스 스폰
		ACustomPortal* NewPortal = GetWorld()->SpawnActor<ACustomPortal>(PortalClass, SpawnLocation, SpawnRotation, SpawnParams);
        
		if (NewPortal)
		{
			NewPortal->PortalID = ColorIndex;
            
			// 관리용 변수에 저장 및 디버그 라인 표시
			if (ColorIndex == 0) BluePortal = NewPortal;
			else OrangePortal = NewPortal;
			
			// 연결 가능한 포탈이 있다면 내부 변수로 저장하기
			if (BluePortal && OrangePortal)
			{
				BluePortal->LinkedPortal = OrangePortal;
				OrangePortal->LinkedPortal = BluePortal;
				
				// 화면에 성공 메시지 출력
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(
						-1,                 // Key: -1은 메시지를 덮어쓰지 않고 계속 새로 출력
						5.0f,               // TimeToDisplay: 5초 동안 표시
						FColor::Green,       // DisplayColor: 연결 성공을 알리는 청록색
						TEXT("Portals Linked Successfully!") // DebugMessage
					);
				}
			}

			// 시각적 피드백: 총구 소켓 위치 가져오기
			FVector MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);
			// 발사 방향에 따른 디버그 라인 (파랑/주황 구분)
			FColor DebugColor = (ColorIndex == 0) ? FColor::Blue : FColor::Orange;
			DrawDebugLine(GetWorld(), MuzzleLoc, PlayerHitResult.ImpactPoint, DebugColor, false, 1.0f, 0, 2.0f);
		}

		// 7. AI 인지 시스템에 소음 전달 (ArenaShooter 이식)
		MakeNoise(ShotLoudness, PortalHoldingPlayer, GetActorLocation(), ShotNoiseRange);
	}
    //}
	// else
	// {
	// 	if (GEngine)
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(
	// 		-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
	// 		3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
	// 		FColor::Red, // DisplayColor: 텍스트 색상
	// 		TEXT("Can't make Portal!") // DebugMessage: 출력할 문자열
	// 		);
	// 	}
	// }
}

const TSubclassOf<UAnimInstance>& APortalGun::GetFirstPersonAnimInstanceClass() const
{
	return FirstPersonAnimInstanceClass;
}

const TSubclassOf<UAnimInstance>& APortalGun::GetThirdPersonAnimInstanceClass() const
{
	return ThirdPersonAnimInstanceClass;
}

// Called every frame
// void APortalGun::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

