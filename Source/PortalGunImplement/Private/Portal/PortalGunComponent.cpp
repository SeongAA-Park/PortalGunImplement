// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalGunComponent.h"

#include "PortalGunImplementCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPortalGunComponent::UPortalGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPortalGunComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//캐릭터의 델리게이트 연결
	if (APortalGunImplementCharacter* PlayCharacter = Cast<APortalGunImplementCharacter>(GetOwner()))
	{
		PlayCharacter->OnPortalShot.AddDynamic(this, &ThisClass::HandlePortalShot);
	}

	// ...
}

void UPortalGunComponent::HandlePortalShot(int32 PortalColorIndex)
{
	ExecutePortalTrace(PortalColorIndex);
}

void UPortalGunComponent::ExecutePortalTrace(int32 ColorIndex)
{
	APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // = GetWorld()->GetFirstPlayerController()
	if (!PC) return;
	
	//카메라의 위치와 회전(조준 방향) 가져오기
	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	//조준점(카메라 정중앙) 기준 레이캐스트 시작/끝점 계산
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + CameraRotation.Vector()*5000.f;
	
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());  //플레이어 자신은 통과
	
	//라인트레이스 생성 및 발사
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
	{
		//충돌 지점
		FVector ImpactPoint = HitResult.ImpactPoint;
		//컴포넌트(총구쪽에 붙일 예정)의 빔 발사 지점
		FVector MuzzlePoint = GetComponentLocation();
		//빔 발사 이펙트를 위한 방향 계산
		FVector PortalBeamDirection = (ImpactPoint - MuzzlePoint).GetSafeNormal();
		
		//디버그를 위한 라인 표시
		DrawDebugLine(GetWorld(),MuzzlePoint, ImpactPoint, FColor::Red, false,1.0f, 0, 1.f );
		//이후 포탈 이펙트 추가하기
	}
}

// Called every frame
// void UPortalGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }

