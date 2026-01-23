// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Portal/CustomPortal.h"
#include "Logging/LogMacros.h"
#include "PortalGunImplementCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// 델리게이트 선언 (정수형 매개변수: 0은 파란색, 1은 주황색 포탈)
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPortalShotSignature, int32, PortalColorIndex);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class APortalGunImplementCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;  //원래 앞에 class가 붙어있었다. 위에 class UInputAction이 전방선언되어 있음에도 불구하고...

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MouseLookAction;
	
	
	
	// ~~~추가함~~~
	// about Portal Action
	// UPROPERTY(EditAnywhere, Category ="Input|Portal")
	// TObjectPtr<UInputAction> ShootBluePT;
	//
	// UPROPERTY(EditAnywhere, Category ="Input|Portal")
	// TObjectPtr<UInputAction> ShootOrangePT;
	//
	// /**PortalGun 조작에 관한 Input Action*/
	// UPROPERTY(EditAnywhere, Category ="Input")
	// TObjectPtr<class UInputMappingContext> PortalMappingContext;
	
	
	
public:
	APortalGunImplementCharacter();
	
public:
	//포탈건 컴포넌트가 구독할 방송 신호 변수
	// UPROPERTY(BlueprintAssignable, Category = "Portal")
	// FOnPortalShotSignature OnPortalShot;

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	//포탈 관련 action 입력 시 실행될 함수들
	void InputShootBluePT();
	void InputShootOrangePT();

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
//about Portal	
public:
	//파란색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasBlueGun = false;
	
	//주황색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasOrangeGun = false;
	
};

