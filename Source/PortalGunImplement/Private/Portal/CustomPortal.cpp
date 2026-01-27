// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/CustomPortal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ACustomPortal::ACustomPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//나중에 카메라 위치 계산을 위해 Tick을 킨다.
	
	//포탈 메쉬 생성 및 루트로 설정
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	RootComponent = PortalMesh;
	
	//포탈 메쉬 : 기본 Plane 메쉬로 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		PortalMesh->SetStaticMesh(PlaneMeshAsset.Object);
	}
	//포탈 외형 조정 (회전 및 직사각형비율)
	PortalMesh->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	PortalMesh->SetRelativeScale3D(FVector(3.f, 2.f, 1.0f));
	
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
	
	// 고유한 렌더 타겟 설정 (해상도는 성능에 따라 조절)
	UTextureRenderTarget2D* NewRT = NewObject<UTextureRenderTarget2D>(this);
	NewRT->InitAutoFormat(1024, 1024); //1024 해상도 권장
	NewRT->ClearColor = FLinearColor::Gray;
	
	//내 카메라에 렌더 타겟 연결
	if (PortalCamera)
	{
		PortalCamera->TextureTarget = NewRT;
	}
	
	//다이나믹 매터리얼 인스턴스 생성 및 텍스쳐 전달
	if (PortalMesh)
	{
		UMaterialInstanceDynamic* DynMat = PortalMesh->CreateDynamicMaterialInstance(0);
		if (DynMat)
		{
			DynMat->SetTextureParameterValue(TEXT("PortalRendTex"),NewRT);
		}
	}
}

// Called every frame
void ACustomPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomPortal::UpdatePortalView()
{
	// if (!LinkedPortal || !PortalCamera) return;
	//
	// //플레이어 카메라 정보 가져오기
	// APlayerCameraManager* CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//
	// FVector CamLocation = CamManager->GetCameraLocation();
	// FRotator CamRotation = CamManager->GetCameraRotation();
	// FTransform PlayerCamTransform(CamRotation, CamLocation);
	//
	// // 플레이어와 포탈A간의 상대적 위치 계산
	// FTransform CharNPotARelativeTransform = PlayerCamTransform.GetRelativeTransform(GetActorTransform());
	//
	// // 거울과 같은 view를 위해 Flip
	// FTransform FlipViewTransform = 
	
}

