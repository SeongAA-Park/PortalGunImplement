// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalGunComponent.h"

#include "PortalGunImplementCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
		//PlayCharacter->OnPortalShot.AddDynamic(this, &ThisClass::HandlePortalShot);
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
		
		//라인트레이스 충돌 지점에 포탈 설치
		//스폰 위치: 벽에 닿은 지점에서 아주 미세하게 앞(normal 방향)으로 빼주기
		FVector SpawnLocation = ImpactPoint+ (HitResult.ImpactNormal*0.01f);
		
		//스폰될 포탈 회전시키기 : 벽의 Normal을 포탈 Normal과 맞추기?  << 다른 로직도 고려?
		FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);
		
		//기존에 해당 색상의 포탈이 있었다면 제거하기
		if (ColorIndex == 0 && BluePortal) BluePortal->Destroy();
		else if (ColorIndex == 1 && OrangePortal) OrangePortal->Destroy();
		
		//액터 스폰 설정 (재검토 필요)
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		
		//실제 포탈 소환
		ACustomPortal* NewPortal = GetWorld()->SpawnActor<ACustomPortal>(PortalClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewPortal)
		{
			NewPortal->PortalID = ColorIndex;  //포탈액터 자체는 파랑/주황 모두 포함... ColorIndex는 설정해줘야함
			
			//관리용 변수에 저장하기
			if (ColorIndex == 0) BluePortal = NewPortal;
			else OrangePortal = NewPortal;
		}
	}
}

// Called every frame
// void UPortalGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }

