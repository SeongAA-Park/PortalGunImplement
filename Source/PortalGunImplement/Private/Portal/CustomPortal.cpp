// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/CustomPortal.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h" // Ignore 관련 기능을 위해 필요
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Pawn.h"     // IsA(APawn) 및 MoveIgnore를 위해 필요
#include "Engine/EngineTypes.h"

// Sets default values
ACustomPortal::ACustomPortal()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 아무 회전이 없는 깨끗한 'SceneComponent'를 루트로 설정합니다.
	// 이것이 포탈의 수학적 기준점(Forward=+X, Up=+Z)이 됩니다.
	USceneComponent* DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DummyRoot;

	// 메쉬를 루트에 붙이고 외형만 회전시키기
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(RootComponent);
    
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		PortalMesh->SetStaticMesh(PlaneMeshAsset.Object);
	}
	
	// 시각적 보정만 수행 (Actor의 정면인 +X를 바라보게 세움)
	PortalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 90.0f)); //Y축에 180 회전 수행해줘야함... 안하면 투명한 부분이 표면으로 출력 >> Material Two Side로 해결! >> 근데 그렇게 해도 반대되는 view가 나와서 180도 회전하는거랑 똑같음
	PortalMesh->SetRelativeScale3D(FVector(2.f, 3.f, 1.0f));
	
	// 카메라는 루트에 붙입니다. (메쉬가 돌아가도 영향을 받지 않도록)
	PortalCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCamera"));
	PortalCamera->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAssetLoader(TEXT("/Game/PortalContent/_Custom/_BluePrint/CustomPortal/NewCP_Material.NewCP_Material"));
	if (MaterialAssetLoader.Succeeded())
	{
		NewCP_Material = MaterialAssetLoader.Object;
	}
	
	// about Box Collision Component: 컴포넌트 생성 및 루트에 부착
	DetectBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectBoxCollision"));
	DetectBoxCollision->SetupAttachment(RootComponent);

	// 기본 콜리전 설정
	DetectBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectBoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    
	// 통과 대상 채널들만 Overlap으로 설정
	DetectBoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DetectBoxCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	// 이벤트 바인딩 (함수와 연결)
	DetectBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACustomPortal::OnOverlapBegin);
	DetectBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ACustomPortal::OnOverlapEnd);
	
	//(추가) Clip 평면 설정을 위한 PortalCamera의 Capture Source 설정
	PortalCamera->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
    
	PortalCamera->bCaptureEveryFrame = true;
	PortalCamera->bCaptureOnMovement = true;
}

void ACustomPortal::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	
	if (PortalMesh && NewCP_Material)
	{
		//NewCP_Material : 런타임에서 MaterialInstanceDynamic을 생성할 때 설계도 제시(매터리얼 인터페이스)
		//이를 바탕으로 PortalMID 변수 채우기
		PortalMID = UMaterialInstanceDynamic::Create(NewCP_Material, this);
		PortalMesh->SetMaterial(0,PortalMID);
	}
	
	int32 ViewportX = 0;
	int32 ViewportY = 0;
	float ResolutionScale = 1.f;  // [최적화 Try]50% 해상도 (조절 가능: 0.5 ~ 1.0)
	//UWidgetLayoutLibrary::GetViewportSize(ViewportX, ViewportY);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	
	ViewportX = static_cast<int32>(ViewportSize.X * ResolutionScale);
	ViewportY = static_cast<int32>(ViewportSize.Y * ResolutionScale);
	//에러방지
	ViewportX = FMath::Max(ViewportX, 1);
	ViewportY = FMath::Max(ViewportY, 1);
	
	// Create Render Target 2D and Setting
	PortalRT = NewObject<UTextureRenderTarget2D>(this);
	PortalRT->RenderTargetFormat = RTF_RGBA16f;
	PortalRT->InitAutoFormat(ViewportX,ViewportY);
	PortalRT->UpdateResourceImmediate(true);
	
	// PortalMID : UMaterialInstanceDynamic*
	// PortalRT  : UTextureRenderTarget2D*

	if (PortalMID && PortalRT)
	{
		PortalMID->SetTextureParameterValue(
			FName("PotTex"),   // 머티리얼에 만든 파라미터 이름
			PortalRT
		);
	}
	
	if (PortalCamera && PortalRT)
	{
		// 블루프린트의 "Set Texture Target" 노드와 동일한 역할
		// 타겟: PortalCamera, 텍스처 타겟: PortalRT
		PortalCamera->TextureTarget = PortalRT;
		
		// (선택 사항) 포탈 렌더링 시 흔히 쓰이는 추가 설정들
		// PortalCamera->CaptureSource = SCS_SceneColor (HDR); // 보통 포탈은 SceneColor를 많이 씁니다.
	}
	
}

// Called every frame
void ACustomPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 연결된 포탈이 있고, 내가 플레이어 시야에 보일 때만 상대방 카메라를 내 쪽으로 동기화
	if (LinkedPortal) 
	{
		UpdatePortalView3(); 
		//PortalCamera->CaptureScene();  // 필요할 때만 캡처
	}
	// && WasRecentlyRendered(0.1f)
}


void ACustomPortal::UpdatePortalView3()
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
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), FinalTransform.GetLocation().X, FinalTransform.GetLocation().Y, FinalTransform.GetLocation().Z);
	
	// 내 카메라를 최종 월드 위치로 이동 및 장면 캡처
	//LinkedPortal->PortalCamera->SetRelativeTransform(FinalTransform);
	PortalCamera->SetWorldTransform(FinalTransform);
	
	// 디버그 카메라 그리기 (빨간색, 0.1초 동안 유지되게 하여 매 프레임 갱신)
	//DrawDebugCamera(GetWorld(), FinalTransform.GetLocation(), FinalTransform.Rotator(), PortalCamera->FOVAngle, 1.0f, FColor::Red, false, 0.1f);
	
	// ClipPlane 설정
}

void ACustomPortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 유효성 검사 (AttachedWall이 있는지 먼저 확인)
	if (!OtherActor || !AttachedWall.IsValid()) return;

	// 2. 통과 대상인지 확인 (태그 또는 플레이어)
	bool bHasTag = OtherActor->ActorHasTag(FName("PortalPassable"));
	bool bIsPawn = OtherActor->IsA(APawn::StaticClass());
	const bool bIsPassable = bHasTag || bIsPawn;

	if (bIsPassable)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,                      // Key: -1이면 새 메시지 추가, 특정 숫자면 해당 메시지 갱신
				3.f,                     // TimeToDisplay: 메시지가 떠 있을 시간 (초)
				FColor::Emerald,            // DisplayColor: 텍스트 색상
				TEXT("Overlapped Actor is Passable")     // DebugMessage: 출력할 내용
			);
		}
		
		// 포탈이 붙어있는 벽의 포인터 가져오기
		AActor* WallToIgnore = AttachedWall.Get();
		// 충돌한 액터의 RootComponent 가져오기
		UPrimitiveComponent* OverlapedActorRoot = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		// 추가 : 벽의 루트 컴포넌트 가져오기
		// UPrimitiveComponent* WallRoot = nullptr;
		// if (WallToIgnore)
		// {
		// 	WallRoot = Cast<UPrimitiveComponent>(WallToIgnore->GetRootComponent());
		// }
		// 좀 더 간결하게 수정
		UPrimitiveComponent* WallRoot = WallToIgnore? Cast<UPrimitiveComponent>(WallToIgnore->GetRootComponent()) : nullptr;
		
		//수정
		if (OverlapedActorRoot && WallRoot)
		{
			if (OverlapedActorRoot->IsSimulatingPhysics())
			{
				// portalwall 채널에 대해 Overlap으로 변경
				OverlapedActorRoot->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
				
				UE_LOG(LogTemp, Warning, TEXT("[Portal] Physics Actor %s: Set Overlap to PortalWall"), *OtherActor->GetName());
			}
			else
			{
				// Movement 기반 액터 (플레이어 등)
				OverlapedActorRoot->IgnoreActorWhenMoving(WallToIgnore, true);
				WallRoot->IgnoreActorWhenMoving(OtherActor, true);
        
				UE_LOG(LogTemp, Warning, TEXT("[Portal] Movement Actor %s: Using IgnoreActorWhenMoving"), *OtherActor->GetName());
			}
		}
		
		// 3. 물리 상태 갱신 (자고 있는 물리 엔진 깨우기)
		if (OverlapedActorRoot->IsSimulatingPhysics())
		{
			OverlapedActorRoot->WakeAllRigidBodies();
		}
		
	}
}

void ACustomPortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 1. 유효성 검사
	if (OtherActor && AttachedWall.IsValid())
	{
		AActor* WallToIgnore = AttachedWall.Get();
		UPrimitiveComponent* OverlapedActorRoot = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		UPrimitiveComponent* WallRoot = WallToIgnore ? Cast<UPrimitiveComponent>(WallToIgnore->GetRootComponent()) : nullptr;

		if (!OverlapedActorRoot || !WallRoot) return;

		// Physics 액터: Collision Response 복원
		if (OverlapedActorRoot->IsSimulatingPhysics())
		{
			OverlapedActorRoot->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
			UE_LOG(LogTemp, Log, TEXT("[Portal] Physics Actor %s: Restored Block to PortalWall"), *OtherActor->GetName());
		}
		else
		{
			// Movement 기반 액터
			OverlapedActorRoot->IgnoreActorWhenMoving(WallToIgnore, false);
			WallRoot->IgnoreActorWhenMoving(OtherActor, false);
		}
	}
}


