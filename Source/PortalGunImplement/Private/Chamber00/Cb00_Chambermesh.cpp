// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_Chambermesh.h"

// Sets default values
ACb00_Chambermesh::ACb00_Chambermesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	worldspawn_1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("worldspawn_1	"));
	worldspawn_1->SetupAttachment(Root);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> worldspawnmeshasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/worldspawn_1.worldspawn_1"));
	if (worldspawnmeshasset.Succeeded())
	{
		worldspawn_1->SetStaticMesh(worldspawnmeshasset.Object);
	}
	
	// 4) 트랜스폼 지정 (부모(Root) 기준 상대 트랜스폼)
	worldspawn_1->SetRelativeLocation(FVector(543.972534f, -375.541016f, -297.462433f));
	worldspawn_1->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	worldspawn_1->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));

}

// Called when the game starts or when spawned
void ACb00_Chambermesh::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ACb00_Chambermesh::GetPortalSurfaceActor_Implementation()
{
	return this;
}

// Called every frame
// void ACb00_Chambermesh::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

