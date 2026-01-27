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

	// --- 디버그 라인 추가 시작 ---
	// 충돌 여부에 따라 선 색상 결정 (성공: 녹색, 실패: 빨간색)
	FColor LineColor = bHit ? FColor::Green : FColor::Red;

	// 디버그 라인 그리기
	DrawDebugLine(
		GetWorld(), 
		Start, 
		End, 
		LineColor, 
		false,      // bPersistentLines: 영구 표시 여부
		2.0f,       // LifeTime: 2초 동안 표시
		0,          // DepthPriority
		1.0f        // Thickness: 선 두께
	);

	// 충돌했을 경우 충돌 지점에 작은 구체 표시
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 10.f, 12, FColor::Blue, false, 2.0f);
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
	if (DefaultPortalGunClass)
	{
		// 2. 액터 스폰 설정 (주인과 가해자 설정)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 3. 월드에 포탈건 생성
		// 위치는 캐릭터의 현재 위치로 잡고, 생성 후 바로 부착할 것이므로 회전은 기본값(Zero)으로 충분합니다.
		CurrentWeapon = GetWorld()->SpawnActor<APortalGun>(DefaultPortalGunClass, GetActorTransform(), SpawnParams);

		if (CurrentWeapon)
		{
			// 4. 캐릭터의 손 소켓에 무기 메시 부착
			// ShooterCharacter에서 구현했던 부착 함수를 호출합니다.
			AttachWeaponMeshes(CurrentWeapon);

			// 5. 애니메이션 및 UI 활성화
			// ShooterCharacter에 정의된 활성화 로직을 호출하여 팔의 포즈를 바꿉니다.
			OnPortalGunActivated(CurrentWeapon);

			// 6. 포탈 발사 권한 초기화
			bHasBlueGun = true;
			bHasOrangeGun = true;
            
			UE_LOG(LogTemp, Log, TEXT("Portal Gun successfully spawned and equipped."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultPortalGunClass is NOT assigned in Blueprint!"));
	}
	
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
	}
}

void APortalGunShooterCharacter::InputShootBluePT()
{
	//여기에 포탈건 발사 사운드, 이펙트, 델리게이트 등이 들어갈 수 있습니다. -> Has-a 관계이기에 델리게이트 불필요
	//UE_LOG(LogTemp, Warning, TEXT("Blue Portal Shot! BroadCast PortalColorIndexValue 0"));
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
			3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
			FColor::Blue, // DisplayColor: 텍스트 색상
			TEXT("Blue Portal Shot!") // DebugMessage: 출력할 문자열
		);
	}
	
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
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,          // Key: 동일한 로그를 덮어쓸지 여부 (-1은 매번 새로 출력)
			3.f,         // TimeToDisplay: 화면에 표시될 시간 (초)
			FColor::Orange, // DisplayColor: 텍스트 색상
			TEXT("Orange Portal Shot!") // DebugMessage: 출력할 문자열
		);
	}
	//전달할 값 방송
	//OnPortalShot.Broadcast(1);
	if (CurrentWeapon != nullptr && bHasOrangeGun )
	{
		CurrentWeapon->HandlePortalShot(1);
	}
}

