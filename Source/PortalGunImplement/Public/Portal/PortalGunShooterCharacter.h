// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalGunImplementCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PortalGunShooterCharacter.generated.h"

class UInputAction;
class UInputComponent;
class APortalGun;
/**
 * 
 */
// 델리게이트 선언 (정수형 매개변수: 0은 파란색, 1은 주황색 포탈), from PortalGunImplementCharacter
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPortalShotSignature, int32, PortalColorIndex);

UCLASS()
class PORTALGUNIMPLEMENT_API APortalGunShooterCharacter : public APortalGunImplementCharacter
{
	GENERATED_BODY()
	
	/** AI Noise emitter component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;
	
	//player의 기능 추가 : E 버튼으로 임의의 물체를 허공에 붙잡고 놓을 수 있어야 함
	//Add PhysicsHandle : Physics Handle Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character",meta=(AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* PhysicsHandle; 
	
protected:
	// about Portal Action - Fire BluePortal
	UPROPERTY(EditAnywhere, Category ="Input|Portal")
	TObjectPtr<UInputAction> ShootBluePT;
	
	//Added
	// about Portal Action - Fire OrangePortal
	UPROPERTY(EditAnywhere, Category ="Input|Portal")
	TObjectPtr<UInputAction> ShootOrangePT;
	
	//Added
	//about Portal Action - Grab PhysicsObject
	UPROPERTY(EditAnywhere, Category ="Input|Portal")
	TObjectPtr<UInputAction> GrabPhysicsObject;
	
	//Copied
	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");
	
	//Copied
	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");
	
	//Copied
	/** Max distance to use for aim traces */
	UPROPERTY(EditAnywhere, Category ="Aim", meta = (ClampMin = 0, ClampMax = 100000, Units = "cm"))
	float MaxAimDistance = 10000.0f;
	
	//Copied
	/** Weapon currently equipped and ready to shoot with */
	TObjectPtr<APortalGun> CurrentWeapon;
	
	/** 시작 시 자동으로 생성하여 장착할 포탈건의 블루프린트 클래스 */
	UPROPERTY(EditAnywhere, Category = "Portal | Weapon")
	TSubclassOf<class APortalGun> DefaultPortalGunClass;
	
	/**PortalGun 조작에 관한 Input Action*/
	// UPROPERTY(EditAnywhere, Category ="Input")
	// TObjectPtr<class UInputMappingContext> PortalMappingContext;
	
	//about Portal, from PortalGunImplementCharacter	
public:
	//파란색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasBlueGun = false;
	
	//주황색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasOrangeGun = false;

public:
	//포탈건이 구독할 방송 신호 변수
	// UPROPERTY(BlueprintAssignable, Category = "Portal")
	// FOnPortalShotSignature OnPortalShot;
	
public:
	APortalGunShooterCharacter();
	
public:
	//Copied from Interface
	/** Attaches a weapon's meshes to the owner */
	void AttachWeaponMeshes(APortalGun* Weapon);
	
	/** Calculates and returns the aim location for the weapon, 카메라 정면의 조준 지점을 계산 */
	FHitResult GetWeaponTargetLocation();
	
	void OnPortalGunActivated(APortalGun* Weapon);

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//포탈 관련 action 입력 시 실행될 함수들, from PortalGunImplementCharacter
	void InputShootBluePT();
	
	void InputShootOrangePT();
	
	void InputGrabPhysicsObject();
	
};
