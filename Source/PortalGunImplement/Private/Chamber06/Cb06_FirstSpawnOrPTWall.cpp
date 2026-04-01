// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber06/Cb06_FirstSpawnOrPTWall.h"

// Sets default values
ACb06_FirstSpawnOrPTWall::ACb06_FirstSpawnOrPTWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	//func_detail_8256
	func_detail_16394 = CreateDefaultSubobject<UStaticMeshComponent>(FName("func_detail_16394"));
	func_detail_16394->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> func_detail_16394asset(TEXT("/Game/PortalContent/_Map/Chamber_06/ImportFBX_v3/func_detail_16394.func_detail_16394"));
	if (func_detail_16394asset.Succeeded())
	{
		func_detail_16394->SetStaticMesh(func_detail_16394asset.Object);
	}

	func_detail_16394->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	func_detail_16394->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));

}

// Called when the game starts or when spawned
void ACb06_FirstSpawnOrPTWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
// void ACb06_FirstSpawnOrPTWall::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

AActor* ACb06_FirstSpawnOrPTWall::GetPortalSurfaceActor_Implementation()
{
	return this;
}

