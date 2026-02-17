// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_SpawnGlassroomPTWall.h"

// Sets default values
ACb00_SpawnGlassroomPTWall::ACb00_SpawnGlassroomPTWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	//func_detail_8256
	func_detail_8256 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_8256"));
	func_detail_8256->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_8256asset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/func_detail_8256.func_detail_8256"));
	if (func_detail_8256asset.Succeeded())
	{
		func_detail_8256->SetStaticMesh(func_detail_8256asset.Object);
	}

	func_detail_8256->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));
	
}

// Called when the game starts or when spawned
void ACb00_SpawnGlassroomPTWall::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ACb00_SpawnGlassroomPTWall::GetPortalSurfaceActor_Implementation()
{
	return this;
}

// Called every frame
// void ACb00_SpawnGlassroomPTWall::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

