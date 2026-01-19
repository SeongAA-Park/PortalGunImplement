// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/CustomPortal.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ACustomPortal::ACustomPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//나중에 카메라 위치 계산을 위해 Tick을 킨다.
	
	//포탈 메쉬 생성 및 루트로 설정
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = PortalMesh;
	//포탈 메쉬가 LineTrace에 맞으면 안되므로 충돌 설정을 끈다.
	PortalMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	//씬 캡쳐 카메라 생성 및 부착
	PortalCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCamera"));
	PortalCamera->SetupAttachment(RootComponent);
	
	//[최적화 관련 중요 설정!]성능을 위해 자동으로 모든 것을 찍지 않도록 하기위한 설정, 우리가 필요할 때(Tick에서)만 수동으로 찍게 할 것!
	PortalCamera->bCaptureEveryFrame = false;
	PortalCamera->bCaptureOnMovement = false;
}

// Called when the game starts or when spawned
void ACustomPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

