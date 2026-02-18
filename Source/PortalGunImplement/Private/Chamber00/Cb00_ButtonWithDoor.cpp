// Fill out your copyright notice in the Description page of Project Settings.


#include "Chamber00/Cb00_ButtonWithDoor.h"
#include "Chamber00/Cb00_InternalDoor.h"

#include "Components/BoxComponent.h"

// Sets default values
ACb00_ButtonWithDoor::ACb00_ButtonWithDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	ButtonTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buttontop"));
	ButtonTop->SetupAttachment(Root);
	static::ConstructorHelpers::FObjectFinder<UStaticMesh> Buttontopasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/props_button_top_reference_002.props_button_top_reference_002"));
	if (Buttontopasset.Succeeded())
	{
		ButtonTop->SetStaticMesh(Buttontopasset.Object);
	}
	
	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Buttonbase"));
	ButtonBase->SetupAttachment(Root);
	static::ConstructorHelpers::FObjectFinder<UStaticMesh> Buttonbaseasset(TEXT("/Game/PortalContent/_Map/Chamber_00/ImportFBX_V4/props_button_base_reference.props_button_base_reference"));
	if (Buttonbaseasset.Succeeded())
	{
		ButtonBase->SetStaticMesh(Buttonbaseasset.Object);
	}
	
	ButtonTop->SetRelativeScale3D(FVector(160.f,160.f,160.f));
	ButtonBase->SetRelativeScale3D(FVector(160.f,160.f,160.f));
	
	ButtonTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonTrigger"));
	ButtonTrigger->SetupAttachment(Root);
	
	// Trigger 기본 설정: Pawn, PhysicsBody만 Overlap
	ButtonTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ButtonTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ButtonTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ButtonTrigger->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	ButtonTrigger->SetGenerateOverlapEvents(true);

	// 크기는 BP/디테일에서 조정해도 됨
	ButtonTrigger->SetBoxExtent(FVector(40.f, 40.f, 20.f));
	
}

// Called when the game starts or when spawned
void ACb00_ButtonWithDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ButtonTop)
	{
		ButtonTopInitialRelLoc = ButtonTop->GetRelativeLocation();
	}

	// 이벤트 바인딩
	if (ButtonTrigger)
	{
		ButtonTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACb00_ButtonWithDoor::OnTriggerBeginOverlap);
		ButtonTrigger->OnComponentEndOverlap.AddDynamic(this, &ACb00_ButtonWithDoor::OnTriggerEndOverlap);
	}

	// 시작 상태 정리
	PressingActors.Empty();
	bIsPressed = false;
	SetButtonVisualPressed(false);
}

void ACb00_ButtonWithDoor::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidPresser(OtherActor, OtherComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTriggerBeginOverlap: OtherActor is invalid"));
		return;
	}

	PressingActors.Add(OtherActor);
	UpdatePressedState();
}

void ACb00_ButtonWithDoor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	PressingActors.Remove(OtherActor);
	UpdatePressedState();
}



bool ACb00_ButtonWithDoor::IsValidPresser(AActor* OtherActor, UPrimitiveComponent* OtherComp) const
{
	if (!OtherActor || OtherActor == this) return false;

	// 1) Pawn이면 무조건 OK
	if (OtherActor->IsA<APawn>()) return true;

	// 2) Physics simulation 켜진 물체면 OK
	if (OtherComp && OtherComp->IsSimulatingPhysics()) return true;

	// (보강) OtherComp가 아니면 RootComponent가 물리인지도 체크
	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
	{
		if (RootPrim->IsSimulatingPhysics()) return true;
	}
	
	return false;
}

void ACb00_ButtonWithDoor::UpdatePressedState()
{
	// invalid 약참조 정리
	for (auto It = PressingActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}

	const bool bShouldBePressed = (PressingActors.Num() > 0);

	if (bShouldBePressed == bIsPressed)
	{
		return; // 상태 변화 없음
	}

	bIsPressed = bShouldBePressed;
	SetButtonVisualPressed(bIsPressed);

	if (LinkedDoor)
	{
		if (bIsPressed) LinkedDoor->DoorOpen();
		else            LinkedDoor->DoorClose();
	}
}

void ACb00_ButtonWithDoor::SetButtonVisualPressed(bool bPressed)
{
	if (!ButtonTop) return;

	FVector NewLoc = ButtonTopInitialRelLoc;
	if (bPressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Button Is Pressing!"));
		NewLoc.Z -= PressDepth; // Z축으로 내려감
	}

	ButtonTop->SetRelativeLocation(NewLoc);
}


// Called every frame
void ACb00_ButtonWithDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}