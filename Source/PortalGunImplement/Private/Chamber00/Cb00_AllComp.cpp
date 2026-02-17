// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_AllComp.h"

// Sets default values
ACb00_AllComp::ACb00_AllComp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	worldspawn_1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("worldspawn_1	"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> worldspawnmeshasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/worldspawn_1.worldspawn_1"));
	if (worldspawnmeshasset.Succeeded())
	{
		worldspawn_1->SetStaticMesh(worldspawnmeshasset.Object);
	}
	
	

}

// Called when the game starts or when spawned
void ACb00_AllComp::BeginPlay()
{
	Super::BeginPlay();
	
	if (worldspawn_1)
	{
		// 예: 콜리전 프로필을 'PhysicsActor'로 변경
		worldspawn_1->SetCollisionProfileName(TEXT("worldspawn_1"));
        
		// 또는 특정 채널만 무시/블록 설정
		// MyTargetMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		// 2. Object Type을 PortalWall로 설정 (ECC_GameTraceChannel2)
		worldspawn_1->SetCollisionObjectType(ECC_GameTraceChannel2);
    
		// 3. 기본 응답 설정 (다른 물체들과는 Block)
		worldspawn_1->SetCollisionResponseToAllChannels(ECR_Block);
    
		// 4. Collision Enabled
		worldspawn_1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
		// 생성 시 자동으로 태그 추가 -> 태그 대신 인스턴스 확인
		Tags.Add(FName("PortalPassableWall"));
	}
	
}

AActor* ACb00_AllComp::GetPortalSurfaceActor_Implementation()
{
	return this;
}

// Called every frame
// void ACb00_AllComp::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

