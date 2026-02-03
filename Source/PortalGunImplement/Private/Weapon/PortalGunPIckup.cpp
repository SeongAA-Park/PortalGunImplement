// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PortalGunPIckup.h"
#include "Components/SphereComponent.h"
#include "Portal/PortalGunShooterCharacter.h"

// Sets default values
APortalGunPIckup::APortalGunPIckup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// 루트 컴포넌트 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 충돌체 설정
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(50.0f);
	
	// 충돌 프로필 설정 (Pawn과만 겹침)
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 델리게이트 바인딩
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APortalGunPIckup::OnOverlap);

	// 메쉬 설정
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(SphereCollision);
	PickupMesh->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void APortalGunPIckup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalGunPIckup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 충돌한 대상이 PortalGunShooterCharacter인지 확인
	APortalGunShooterCharacter* PlayerChar = Cast<APortalGunShooterCharacter>(OtherActor);
	
	if (PlayerChar)
	{
		// 2. 픽업 타입에 따라 캐릭터의 권한 변수 업데이트
		if (PickupType == EPortalPickupType::Blue)
		{
			PlayerChar->bHasBlueGun = true;
			// 시각적 피드백이나 로그 (선택 사항)
			UE_LOG(LogTemp, Log, TEXT("Blue Portal Ability Acquired!"));
		}
		else if (PickupType == EPortalPickupType::Orange)
		{
			PlayerChar->bHasOrangeGun = true;
			UE_LOG(LogTemp, Log, TEXT("Orange Portal Ability Acquired!"));
		}

		// 3. 캐릭터의 UI나 외형을 즉시 업데이트하는 함수 호출 (필요 시)
		// PlayerChar->NotifyPortalAbilityChanged();

		// 4. 리스폰 없이 즉시 파괴
		Destroy();
	}
}
// Called every frame
// void APortalGunPIckup::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

