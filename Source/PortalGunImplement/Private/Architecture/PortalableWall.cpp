// Fill out your copyright notice in the Description page of Project Settings.


#include "Architecture/PortalableWall.h"

// Sets default values
APortalableWall::APortalableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	RootComponent = PlaneComponent;
	
	//PlaneComponent->SetCollisionProfileName(FName("BlockAll"));
	//[수정!!] 해당 컴포넌트의 충돌 설정을 Custom으로 변경
	//PlaneComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2,ECR_Block);
	// 위 코드는 잘못된 코드 : Wall이 PortalWall 채널을 가진 물체와 충돌할 때 Block한다는 의미
	
	//다시수정
	// ===== 중요: 충돌 설정 =====
	// 1. Custom Collision Preset 사용
	PlaneComponent->SetCollisionProfileName(TEXT("Custom"));
    
	// 2. Object Type을 PortalWall로 설정 (ECC_GameTraceChannel2)
	PlaneComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
    
	// 3. 기본 응답 설정 (다른 물체들과는 Block)
	PlaneComponent->SetCollisionResponseToAllChannels(ECR_Block);
    
	// 4. Collision Enabled
	PlaneComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// 생성 시 자동으로 태그 추가 -> 태그 대신 인스턴스 확인
	Tags.Add(FName("PortalPassableWall"));
	
}

AActor* APortalableWall::GetPortalSurfaceActor_Implementation()
{
	//인터페이스 호출 시 자기 자신 반환
	return this;
}

// Called when the game starts or when spawned
// void APortalableWall::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }

// Called every frame
// void APortalableWall::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }

