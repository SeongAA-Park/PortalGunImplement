// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortalGunImplementCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PortalGunImplement.h"

APortalGunImplementCharacter::APortalGunImplementCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

//액션 바인딩
void APortalGunImplementCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APortalGunImplementCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APortalGunImplementCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APortalGunImplementCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APortalGunImplementCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APortalGunImplementCharacter::LookInput);
		
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
	else
	{
		UE_LOG(LogPortalGunImplement, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APortalGunImplementCharacter::ShootPortal(int32 PortalID)
{
	//권한 체크 : 특정 포탈건 발사 시도인데 권한이 없으면 그냥 return
	if (PortalID == 0 && !bHasBlueGun) return;   // 파란색 발사 시도인데 권한 없으면 취소
	if (PortalID == 1 && !bHasOrangeGun) return; // 주황색 발사 시도인데 권한 없으면 취소
	
	//카메라의 현재 위치와 회전값(바라보는 방향)을 가져온다.
	FVector Start;
	FRotator Rotation;
	GetActorEyesViewPoint(Start, Rotation);
	
	//레이저의 끝점 계산 (충분히 긴 거리만큼 쏘기)
	FVector End = Start + (Rotation.Vector()*5000.f);
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  //레이저가 캐릭터 자기 자신에게 맞는 것을 방지
	
	//월드에서 레이저(Line Trace)를 쏩니다
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		//벽의 경사면을 사용하여 포탈이 벽에 딱 붙을 회전값 계산
		//벽면에서 수직으로 튀어나오는 방향(X축)을 기준으로 회전 행렬 만들기
		FRotator PortalRotation = FRotationMatrix::MakeFromX(HitResult.Normal).Rotator();
		FVector PortalLocation = HitResult.Location;
		
		//어떤 포탈(파랑/주황)을 조작할지 결정
		//-----이중 포인터 헷갈릴 수 있으니 다시보기.-----
		TObjectPtr<ACustomPortal>* TargetPortal = (PortalID == 0) ? &BluePortal : &OrangePortal;  
		
		if (*TargetPortal == nullptr)
		{
			//월드에 해당 포탈이 없다면 새로 소환(Spawn)
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			*TargetPortal = GetWorld()->SpawnActor<ACustomPortal>(PortalClass, PortalLocation, PortalRotation, SpawnParams);
			
			if (*TargetPortal != nullptr)
			{
				(*TargetPortal)->PortalID = PortalID;
			}
		}
		else
		{
			//이미 포탈이 있다면 위치와 회전값만 업데이트
			(*TargetPortal)->SetActorLocationAndRotation(PortalLocation, PortalRotation);
		}
	}
}


void APortalGunImplementCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void APortalGunImplementCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void APortalGunImplementCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APortalGunImplementCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void APortalGunImplementCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void APortalGunImplementCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void APortalGunImplementCharacter::InputShootBluePT()
{
	//여기에 포탈건 발사 사운드, 이펙트, 델리게이트 등이 들어갈 수 있습니다.
	UE_LOG(LogTemp, Warning, TEXT("Blue Portal Shot!"));
}

void APortalGunImplementCharacter::InputShootOrangePT()
{
	//여기에 포탈건 발사 사운드, 이펙트, 델리게이트 등이 들어갈 수 있습니다.
	UE_LOG(LogTemp, Warning, TEXT("Orange Portal Shot!"));
}
