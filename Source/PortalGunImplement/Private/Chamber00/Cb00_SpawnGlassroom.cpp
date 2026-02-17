// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_SpawnGlassroom.h"

// Sets default values
ACb00_SpawnGlassroom::ACb00_SpawnGlassroom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	//func_brush_695277
	func_brush_695277 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695277"));
	func_brush_695277->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695277asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695277.func_brush_695277"));
	if (func_brush_695277asset.Succeeded())
	{
		func_brush_695277->SetStaticMesh(func_brush_695277asset.Object);
	}

	//func_brush_695285
	func_brush_695285 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695285"));
	func_brush_695285->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695285asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695285.func_brush_695285"));
	if (func_brush_695285asset.Succeeded())
	{
		func_brush_695285->SetStaticMesh(func_brush_695285asset.Object);
	}
	
	// func_brush_695293
	func_brush_695293 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695293"));
	func_brush_695293->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695293asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695293.func_brush_695293"));
	if (func_brush_695293asset.Succeeded())
	{
		func_brush_695293->SetStaticMesh(func_brush_695293asset.Object);
	}
	
	//func_brush_695301
	func_brush_695301 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695301"));
	func_brush_695301->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695301asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695301.func_brush_695301"));
	if (func_brush_695301asset.Succeeded())
	{
		func_brush_695301->SetStaticMesh(func_brush_695301asset.Object);
	}
	
	//func_brush_695312
	func_brush_695312 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695312"));
	func_brush_695312->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695312asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695312.func_brush_695312"));
	if (func_brush_695312asset.Succeeded())
	{
		func_brush_695312->SetStaticMesh(func_brush_695312asset.Object);
	}
	
	//func_brush_695320
	func_brush_695320 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695320"));
	func_brush_695320->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695320asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695320.func_brush_695320"));
	if (func_brush_695320asset.Succeeded())
	{
		func_brush_695320->SetStaticMesh(func_brush_695320asset.Object);
	}
	
	//func_brush_695328
	func_brush_695328 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695328"));
	func_brush_695328->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695328asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695328.func_brush_695328"));
	if (func_brush_695328asset.Succeeded())
	{
		func_brush_695328->SetStaticMesh(func_brush_695328asset.Object);
	}
	
	//func_brush_695336
	func_brush_695336 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_brush_695336"));
	func_brush_695336->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_brush_695336asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_brush_695336.func_brush_695336"));
	if (func_brush_695336asset.Succeeded())
	{
		func_brush_695336->SetStaticMesh(func_brush_695336asset.Object);
	}
	
	//func_detail_5701
	func_detail_5701 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_5701"));
	func_detail_5701->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_5701asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_5701.func_detail_5701"));
	if (func_detail_5701asset.Succeeded())
	{
		func_detail_5701->SetStaticMesh(func_detail_5701asset.Object);
	}
	
	//func_detail_5861
	func_detail_5861 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_5861"));
	func_detail_5861->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_5861asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_5861.func_detail_5861"));
	if (func_detail_5861asset.Succeeded())
	{
		func_detail_5861->SetStaticMesh(func_detail_5861asset.Object);
	}
	
	//func_detail_5933
	func_detail_5933 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_5933"));
	func_detail_5933->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_5933asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_5933.func_detail_5933"));
	if (func_detail_5933asset.Succeeded())
	{
		func_detail_5933->SetStaticMesh(func_detail_5933asset.Object);
	}
	
	//func_detail_6113
	func_detail_6113 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_6113"));
	func_detail_6113->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_6113asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_6113.func_detail_6113"));
	if (func_detail_6113asset.Succeeded())
	{
		func_detail_6113->SetStaticMesh(func_detail_6113asset.Object);
	}
	
	//func_detail_6637
	func_detail_6637 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_6637"));
	func_detail_6637->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_6637asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_6637.func_detail_6637"));
	if (func_detail_6637asset.Succeeded())
	{
		func_detail_6637->SetStaticMesh(func_detail_6637asset.Object);
	}
	
	//func_detail_6820
	func_detail_6820 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_6820"));
	func_detail_6820->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_6820asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_6820.func_detail_6820"));
	if (func_detail_6820asset.Succeeded())
	{
		func_detail_6820->SetStaticMesh(func_detail_6820asset.Object);
	}
	
	//func_detail_6917
	func_detail_6917 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_6917"));
	func_detail_6917->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_6917asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_6917.func_detail_6917"));
	if (func_detail_6917asset.Succeeded())
	{
		func_detail_6917->SetStaticMesh(func_detail_6917asset.Object);
	}
	
	//func_detail_7430
	func_detail_7430 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_7430"));
	func_detail_7430->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_7430asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_7430.func_detail_7430"));
	if (func_detail_7430asset.Succeeded())
	{
		func_detail_7430->SetStaticMesh(func_detail_7430asset.Object);
	}
	
	//func_detail_7446
	func_detail_7446 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_7446"));
	func_detail_7446->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_7446asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_7446.func_detail_7446"));
	if (func_detail_7446asset.Succeeded())
	{
		func_detail_7446->SetStaticMesh(func_detail_7446asset.Object);
	}
	
	//func_detail_7508
	func_detail_7508 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_7508"));
	func_detail_7508->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_7508asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_7508.func_detail_7508"));
	if (func_detail_7508asset.Succeeded())
	{
		func_detail_7508->SetStaticMesh(func_detail_7508asset.Object);
	}
	
	//func_detail_7778
	func_detail_7778 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_7778"));
	func_detail_7778->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_7778asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_7778.func_detail_7778"));
	if (func_detail_7778asset.Succeeded())
	{
		func_detail_7778->SetStaticMesh(func_detail_7778asset.Object);
	}
	
	//func_detail_7865
	func_detail_7865 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_7865"));
	func_detail_7865->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_7865asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_7865.func_detail_7865"));
	if (func_detail_7865asset.Succeeded())
	{
		func_detail_7865->SetStaticMesh(func_detail_7865asset.Object);
	}
	
	//func_detail_8256
	func_detail_8256 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_8256"));
	func_detail_8256->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_8256asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_8256.func_detail_8256"));
	if (func_detail_8256asset.Succeeded())
	{
		func_detail_8256->SetStaticMesh(func_detail_8256asset.Object);
	}
	
	//Scale설정
	func_brush_695277->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695285->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695293->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695301->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695312->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695320->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695328->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_brush_695336->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	
	func_detail_5701->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_5861->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_5933->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_6113->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_6637->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_6820->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_6917->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_7430->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_7446->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_7508->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_7778->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_7865->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	func_detail_8256->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
}

// Called when the game starts or when spawned
void ACb00_SpawnGlassroom::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ACb00_SpawnGlassroom::GetPortalSurfaceActor_Implementation()
{
	return this;
}

// Called every frame
// void ACb00_SpawnGlassroom::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

