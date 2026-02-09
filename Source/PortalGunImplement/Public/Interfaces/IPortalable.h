// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPortalable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPortalable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTALGUNIMPLEMENT_API IIPortalable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * 포탈이 설치된 실제 액터를 반환합니다.
	 * BlueprintNativeEvent: C++에서 기본 구현을 가질 수도 있고, 블루프린트에서 오버라이드할 수도 있습니다.
	 * // 함수 반환 타입은 범용성을 위해 원시 포인터 사용
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Portal")
	AActor* GetPortalSurfaceActor();
};
