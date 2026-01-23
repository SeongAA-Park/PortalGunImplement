// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalGunImplementCharacter.h"
#include "PortalGunShooterCharacter.generated.h"

class AShooterWeapon;
/**
 * 
 */
// 델리게이트 선언 (정수형 매개변수: 0은 파란색, 1은 주황색 포탈)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPortalShotSignature, int32, PortalColorIndex);

UCLASS()
class PORTALGUNIMPLEMENT_API APortalGunShooterCharacter : public APortalGunImplementCharacter
{
	GENERATED_BODY()
	
	/** AI Noise emitter component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UPawnNoiseEmitterComponent* PawnNoiseEmitter;
	
protected:
	//Added
	// about Portal Action - Fire BluePortal
	UPROPERTY(EditAnywhere, Category ="Input|Portal")
	TObjectPtr<UInputAction> ShootBluePT;
	
	//Added
	// about Portal Action - Fire OrangePortal
	UPROPERTY(EditAnywhere, Category ="Input|Portal")
	TObjectPtr<UInputAction> ShootOrangePT;
	
	//Copied
	/** Name of the first person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");
	
	//Copied
	/** Name of the third person mesh weapon socket */
	UPROPERTY(EditAnywhere, Category ="Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");
	
	//Copied
	/** Weapon currently equipped and ready to shoot with */
	TObjectPtr<AShooterWeapon> CurrentWeapon;
	
	/**PortalGun 조작에 관한 Input Action*/
	UPROPERTY(EditAnywhere, Category ="Input")
	TObjectPtr<class UInputMappingContext> PortalMappingContext;

public:
	//포탈건이 구독할 방송 신호 변수
	UPROPERTY(BlueprintAssignable, Category = "Portal")
	FOnPortalShotSignature OnPortalShot;
	
public:
	APortalGunShooterCharacter();
	
public:
	//Copied from Interface
	/** Attaches a weapon's meshes to the owner */
	void AttachWeaponMeshes(AShooterWeapon* Weapon);
	
	/** Calculates and returns the aim location for the weapon, 카메라 정면의 조준 지점을 계산 */
	FVector GetWeaponTargetLocation();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//포탈 관련 action 입력 시 실행될 함수들
	void InputShootBluePT();
	
	void InputShootOrangePT();
	
};
