// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/CustomPortal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

// Sets default values
ACustomPortal::ACustomPortal()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 아무 회전이 없는 깨끗한 'SceneComponent'를 루트로 설정합니다.
	// 이것이 포탈의 수학적 기준점(Forward=+X, Up=+Z)이 됩니다.
	USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DummyRoot;

	// 2. 메쉬를 루트에 붙이고 외형만 회전시킵니다.
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootComponent);
    
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		PortalMesh->SetStaticMesh(PlaneMeshAsset.Object);
	}
	
	// 시각적 보정만 수행 (Actor의 정면인 +X를 바라보게 세움)
	PortalMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f)); //Y축에 180 회전 수행해줘야함... 안하면 투명한 부분이 표면으로 출력 >> Material Two Side로 해결! >> 근데 그렇게 해도 반대되는 view가 나와서 180도 회전하는거랑 똑같음
	PortalMesh->SetRelativeScale3D(FVector(3.f, 2.f, 1.0f));
	
	// 3. 카메라는 루트에 붙입니다. (메쉬가 돌아가도 영향을 받지 않도록)
	PortalCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCamera"));
	PortalCamera->SetupAttachment(RootComponent);
	
	//(추가) Clip 평면 설정을 위한 PortalCamera의 Capture Source 설정
	PortalCamera->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
    
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

	// 연결된 포탈이 있고, 내가 플레이어 시야에 보일 때만 상대방 카메라를 내 쪽으로 동기화
	if (LinkedPortal && WasRecentlyRendered(0.1f)) 
	{
		UpdatePortalView();
	}
}

void ACustomPortal::UpdatePortalView()
{
	if (!LinkedPortal || !PortalCamera) return;
	
	//플레이어 카메라 정보 가져오기
	APlayerCameraManager* CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FTransform PlayerCamTransform(CamManager->GetCameraRotation(),  CamManager->GetCameraLocation());
	
	// 플레이어와 포탈A간의 상대적 위치 계산
	FTransform CharNCurrentPotRelativeTransform = PlayerCamTransform.GetRelativeTransform(GetActorTransform());
	
	// 쿼터니언을 이용해 상대 위치와 회전을 거울상으로 뒤집습니다.
	FQuat FlipQuat(FVector(0, 0, 1), FMath::DegreesToRadians(180.f));
	FVector MirroredLocation = FlipQuat.RotateVector(CharNCurrentPotRelativeTransform.GetLocation());
	FQuat MirroredRotation = FlipQuat * CharNCurrentPotRelativeTransform.GetRotation();
	FTransform MirroredTransform(MirroredRotation, MirroredLocation);
	
	// 반전된 시점을 상대방 포탈(B)의 월드 위치로 투영 (Mirrored * M_PortalB)
	FTransform FinalTransform = MirroredTransform * LinkedPortal->GetActorTransform();
	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), FinalTransform.GetLocation().X, FinalTransform.GetLocation().Y, FinalTransform.GetLocation().Z);
	
	// 내 카메라를 최종 월드 위치로 이동 및 장면 캡처
	//LinkedPortal->PortalCamera->SetRelativeTransform(FinalTransform);
	PortalCamera->SetWorldTransform(FinalTransform);
	
	// 디버그 카메라 그리기 (빨간색, 0.1초 동안 유지되게 하여 매 프레임 갱신)
	DrawDebugCamera(GetWorld(), FinalTransform.GetLocation(), FinalTransform.Rotator(), PortalCamera->FOVAngle, 1.0f, FColor::Red, false, 0.1f);
	
	//about Clip Plane 설정
	{
		//클립 평면 설정
		PortalCamera->bEnableClipPlane = true;
		
		// 상대편 포탈의 위치를 기준으로 ClipPlane 설정 (해당 위치에서 바로 자르지 말고 조금 띄우는 것도 방법이여!)
		PortalCamera->ClipPlaneBase = LinkedPortal->GetActorTransform().GetLocation()+LinkedPortal->GetActorForwardVector()*0.1;
	
		// ClipPlane 방향 설정(NormalVector 지정) - 상대편 포탈의 전방 벡터를 Normal로 지정
		//PortalCamera->ClipPlaneNormal = LinkedPortal->GetActorUpVector();
		//PortalCamera->ClipPlaneNormal = PortalCamera->GetForwardVector();
		PortalCamera->ClipPlaneNormal = LinkedPortal->GetActorForwardVector();
	}
	
	// 장면 캡처 실행 (수동 호출)
	//LinkedPortal->PortalCamera->CaptureScene();
	PortalCamera->CaptureScene();
	
	
}

