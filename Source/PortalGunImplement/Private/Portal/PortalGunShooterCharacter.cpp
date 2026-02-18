// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalGunShooterCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/PortalGun.h"

APortalGunShooterCharacter::APortalGunShooterCharacter()
{
	// create the noise emitter component
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Pawn Noise Emitter"));

	// configure movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	
	//physicshandlecomponent 추가
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PortalGunPlayerPhysicsHandle"));
	PhysicsHandle->LinearDamping = 200.0f;
}

void APortalGunShooterCharacter::AttachWeaponMeshes(APortalGun* Weapon)
{
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

	// attach the weapon actor
	Weapon->AttachToActor(this, AttachmentRule);

	// attach the weapon meshes
	Weapon->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Weapon->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);
}

FHitResult APortalGunShooterCharacter::GetWeaponTargetLocation()
{
	// trace ahead from the camera viewpoint
	FHitResult OutHit;

	const FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector End = Start + (GetFirstPersonCameraComponent()->GetForwardVector() * MaxAimDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// // --- 디버그 라인 추가 시작 ---
	// // 충돌 여부에 따라 선 색상 결정 (성공: 녹색, 실패: 빨간색)
	// FColor LineColor = bHit ? FColor::Green : FColor::Red;
	//
	// // 디버그 라인 그리기
	// DrawDebugLine(
	// 	GetWorld(), 
	// 	Start, 
	// 	End, 
	// 	LineColor, 
	// 	false,      // bPersistentLines: 영구 표시 여부
	// 	1.0f,       // LifeTime: 2초 동안 표시
	// 	0,          // DepthPriority
	// 	0.05f        // Thickness: 선 두께
	// );

	// 충돌했을 경우 충돌 지점에 작은 구체 표시
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 8.f, 12, FColor::Blue, false, 1.0f);
	}
	// --- 디버그 라인 추가 끝 ---
	
	// return either the impact point or the trace end
	//return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	return OutHit;
}

void APortalGunShooterCharacter::OnPortalGunActivated(APortalGun* Weapon)
{
	// set the character mesh AnimInstances
	GetFirstPersonMesh()->SetAnimInstanceClass(Weapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Weapon->GetThirdPersonAnimInstanceClass());
}

void APortalGunShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 스폰할 클래스(틀)가 에디터에서 잘 설정되었는지 확인
	// if (DefaultPortalGunClass)
	// {
	// 	// 2. 액터 스폰 설정 (주인과 가해자 설정)
	// 	FActorSpawnParameters SpawnParams;
	// 	SpawnParams.Owner = this;
	// 	SpawnParams.Instigator = this;
	// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	// 	// 3. 월드에 포탈건 생성
	// 	// 위치는 캐릭터의 현재 위치로 잡고, 생성 후 바로 부착할 것이므로 회전은 기본값(Zero)으로 충분합니다.
	// 	CurrentWeapon = GetWorld()->SpawnActor<APortalGun>(DefaultPortalGunClass, GetActorTransform(), SpawnParams);
	//
	// 	if (CurrentWeapon)
	// 	{
	// 		// 4. 캐릭터의 손 소켓에 무기 메시 부착
	// 		// ShooterCharacter에서 구현했던 부착 함수를 호출합니다.
	// 		AttachWeaponMeshes(CurrentWeapon);
	//
	// 		// 5. 애니메이션 및 UI 활성화
	// 		// ShooterCharacter에 정의된 활성화 로직을 호출하여 팔의 포즈를 바꿉니다.
	// 		OnPortalGunActivated(CurrentWeapon);
	//
	// 		// 6. 포탈 발사 권한 초기화
	// 		bHasBlueGun = true;
	// 		bHasOrangeGun = true;
 //            
	// 		UE_LOG(LogTemp, Log, TEXT("Portal Gun successfully spawned and equipped."));
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("DefaultPortalGunClass is NOT assigned in Blueprint!"));
	// }
	
	// 이제 무기는 레벨에서 주워야 하므로 여기서 스폰/장착하지 않음
	bHasBlueGun = false;
	bHasOrangeGun = false;
	
}

void APortalGunShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// base class handles move, aim and jump inputs
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 원래 PortalGunImplementCharacter에 적용했던 InputActionBinding, 해당 InputAction은 PortalGun을 사용하는 Character의 기능이기에 코드이주
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//-----포탈 액션에 관한 값 추가-----
		if (ShootBluePT)
		{
			EnhancedInputComponent->BindAction(ShootBluePT, ETriggerEvent::Started, this, &ThisClass::InputShootBluePT);
		}
	
		if (ShootOrangePT)
		{
			EnhancedInputComponent->BindAction(ShootOrangePT, ETriggerEvent::Started, this, &ThisClass::InputShootOrangePT);
		}
		
		//새로 추가
		if (GrabPhysicsObject)
		{
			EnhancedInputComponent->BindAction(GrabPhysicsObject, ETriggerEvent::Started, this, &ThisClass::InputGrabPhysicsObject);
		}
	}
}

void APortalGunShooterCharacter::InputShootBluePT()
{
	//여기에 포탈건 발사 사운드, 이펙트, 델리게이트 등이 들어갈 수 있습니다. -> Has-a 관계이기에 델리게이트 불필요
	//UE_LOG(LogTemp, Warning, TEXT("Blue Portal Shot! BroadCast PortalColorIndexValue 0"));
	
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
	// 		3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
	// 		FColor::Blue, // DisplayColor: 텍스트 색상
	// 		TEXT("Blue Portal Shot!") // DebugMessage: 출력할 문자열
	// 	);
	// }
	
	//전달할 값 방송
	//OnPortalShot.Broadcast(0);
	if (CurrentWeapon != nullptr && bHasBlueGun )
	{
		CurrentWeapon->HandlePortalShot(0);
	}
}

void APortalGunShooterCharacter::InputShootOrangePT()
{
	//여기에 포탈건 발사 사운드, 이펙트, 델리게이트 등이 들어갈 수 있습니다.
	//UE_LOG(LogTemp, Warning, TEXT("Orange Portal Shot! BroadCast PortalColorIndexValue 1"));
	
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
	// 		3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
	// 		FColor::Orange, // DisplayColor: 텍스트 색상
	// 		TEXT("Orange Portal Shot!") // DebugMessage: 출력할 문자열
	// 	);
	// }
	//전달할 값 방송
	//OnPortalShot.Broadcast(1);
	if (CurrentWeapon != nullptr && bHasOrangeGun )
	{
		CurrentWeapon->HandlePortalShot(1);
	}
}

void APortalGunShooterCharacter::TryPickupPortalGun(APortalGun* PickupGun)
{
	if (!IsValid(PickupGun)) return;

	// 1) 파란 총 픽업: 무기가 없을 때만 장착 + 애니/부착
	if (PickupGun->PickupType == EPortalGunPickupType::Blue)
	{
		if (IsValid(CurrentWeapon))
		{
			// 이미 무기 있으면 파란 총은 무시(정책)
			return;
		}

		CurrentWeapon = PickupGun;

		// PortalGun은 ExecutePortalTrace에서 PortalHoldingPlayer 필요
		PickupGun->SetOwner(this);
		// PortalHoldingPlayer는 PortalGun 내부 protected라 직접 못 건드림 → 대신 BeginPlay 캐싱에 의존하면 늦음
		// 그래서 가장 안전하게 PortalGun에 "SetHoldingPlayer" 함수를 추가하는게 좋음.
		// 여기서는 최소 수정으로: PortalGun BeginPlay 캐싱 말고, HandlePortalShot 시작에서 Owner 캐싱하도록 보완(아래 4번)

		AttachWeaponMeshes(CurrentWeapon);
		OnPortalGunActivated(CurrentWeapon);

		bHasBlueGun = true;
		bHasOrangeGun = false; // 기본은 false 유지

		// 월드 픽업 상태 정리(더이상 겹치지 않게)
		PickupGun->OnPickedUpBy(this);
		return;
	}

	// 2) 오렌지 업그레이드 픽업:
	//    무기 상태 유지(장착/애니 변화 없음) + 기능만 추가 + 월드 총은 사라짐
	if (PickupGun->PickupType == EPortalGunPickupType::OrangeUpgrade)
	{
		// 파란 총을 먼저 먹는 흐름이라면 무기 없을 때는 무시
		if (!IsValid(CurrentWeapon))
		{
			return;
		}

		// 이미 오렌지 권한이 있으면 중복 픽업 무시
		if (bHasOrangeGun)
		{
			return;
		}

		bHasOrangeGun = true;

		// 월드에 있던 오렌지 총은 소모 → 사라짐
		PickupGun->Destroy();
		return;
	}
}


void APortalGunShooterCharacter::InputGrabPhysicsObject_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
			3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
			FColor::Green, // DisplayColor: 텍스트 색상
			TEXT("Pressed E Button, Grap Physics Object") // DebugMessage: 출력할 문자열
		);
	}
}

// void APortalGunShooterCharacter::InputGrabPhysicsObject()
// {
// 	if (GEngine)
// 	{
// 		GEngine->AddOnScreenDebugMessage(
// 			-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
// 			3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
// 			FColor::Green, // DisplayColor: 텍스트 색상
// 			TEXT("Pressed E Button, Grap Physics Object") // DebugMessage: 출력할 문자열
// 		);
// 	}
// 	
// 	//캐릭터 쪽에 physicshandle 관련 함수 만들기...여기가 캐릭터구나
// 	
// }

