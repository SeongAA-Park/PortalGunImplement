// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/IPortalable.h"

// Add default functionality here for any IIPortalable functions that are not pure virtual.

// 기본 구현: 특별한 로직이 없다면 자기 자신(Interface를 상속받은 Actor)을 반환하도록 설정할 수 있습니다.
// 하지만 인터페이스 자체는 Actor가 아니므로, 실제 구현은 상속받는 클래스(BP_Wall 등)에서 하는 것이 정석입니다.
// AActor* IIPortalable::GetPortalSurfaceActor_Implementation()
// {
// 	// 기본적으로는 nullptr을 반환하거나, 상속받은 클래스에서 구현하도록 비워둡니다.
// 	//return nullptr; 
// }