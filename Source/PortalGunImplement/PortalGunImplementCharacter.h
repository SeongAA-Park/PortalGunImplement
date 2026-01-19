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
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

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
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
public:
	APortalGunImplementCharacter();

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

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
public:
	//포탈을 발사하는 핵심 함수입니다. PortalID가 0이면 파란색, 1이면 주황색 포탈을 쏩니다.
	UFUNCTION(BlueprintCallable, Category="Portal")
	void ShootPortal(int32 PortalID);
	
	//블루프린트 에디터에서 'BP_CustomPortal'을 선택할 수 있게 해주는 변수입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Portal")
	TSubclassOf<ACustomPortal> PortalClass;
	
	//파란색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasBlueGun = false;
	
	//주황색 포탈을 쏠 수 있는 권한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal|Ability")
	bool bHasOrangeGun = false;
	
	//생성된 파란색 포탈의 주소를 기억합니다.
	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> BluePortal;
	
	//생성된 주황색 포탈의 주소를 기억합니다.
	UPROPERTY(Transient)
	TObjectPtr<ACustomPortal> OrangePortal;
	

};

