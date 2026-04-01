// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber06/Cb06_Chambermesh.h"

// Sets default values
ACb06_Chambermesh::ACb06_Chambermesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	worldspawn_1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("worldspawn_1"));
	worldspawn_1->SetupAttachment(Root);
	
	static::ConstructorHelpers::FObjectFinder<UStaticMesh> worldspawnasset(TEXT("/Game/PortalContent/_Map/Chamber_06/ImportFBX_v3/worldspawn_1.worldspawn_1"));
	if (worldspawnasset.Succeeded())
	{
		worldspawn_1->SetStaticMesh(worldspawnasset.Object);
	}
	
	worldspawn_1->SetRelativeScale3D(FVector(3.048f, 3.048f, 3.048f));

}

// Called when the game starts or when spawned
void ACb06_Chambermesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
// void ACb06_Chambermesh::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

AActor* ACb06_Chambermesh::GetPortalSurfaceActor_Implementation()
{
	return this;
}

