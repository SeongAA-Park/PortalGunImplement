// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret_Adv.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class USphereComponent;
class UAnimSequence;

UENUM()
enum class ETurretState : uint8
{
	Idle,
	Opening,
	Attacking,
	Closing
};

UCLASS()
class PORTALGUNIMPLEMENT_API ATurret_Adv : public AActor
{
	
	GENERATED_BODY()
	
	public:
    ATurret_Adv();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // =========================
    // Detection (Sphere -> Dot+Trace)
    // =========================
    UFUNCTION()
    void OnDetectionBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                         bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnDetectionEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    bool ConfirmSight(float DeltaTime); // 후보가 있을 때만 Dot+Trace로 최종 판정

    // =========================
    // Aim / Return rotation
    // =========================
    void AimToPlayer(float DeltaTime, const FVector& StartLocation, const FVector& TargetLocation);
    void ReturnToInitialRotation(float DeltaTime);

    // =========================
    // State / Anim
    // =========================
    void SetState(ETurretState NewState);

    void PlayIdle();
    void PlayOpen();
    void PlayClose();

    void OnOpenFinished();
    void OnCloseFinished();

    // =========================
    // Fire
    // =========================
    void StartFire();
    void StopFire();

    UFUNCTION()
    void FireProjectile(); // 타이머 콜백

public:
    // =========================
    // Components
    // =========================
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    // USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USkeletalMeshComponent* TurretSkeletalMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* EyePoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USphereComponent* DetectionSphere;

    // =========================
    // Animations
    // =========================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animations")
    UAnimSequence* IdleAnim;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animations")
    UAnimSequence* OpenAnim;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animations")
    UAnimSequence* CloseAnim;

    // =========================
    // Sockets (Muzzles)
    // =========================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fire")
    FName LeftMuzzleSocketName = TEXT("SK_LFT_Muzzle");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Fire")
    FName RightMuzzleSocketName = TEXT("SK_RT_Muzzle");

    // =========================
    // Settings
    // =========================
    UPROPERTY(EditAnywhere, Category="Detection")
    float DetectionRadius = 1500.f;

    UPROPERTY(EditAnywhere, Category="Sight")
    float SightAngle = 90.f; // degrees

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret Settings")
    float RotationSpeed = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret Settings")
    float FireRate = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turret Settings")
    TSubclassOf<AActor> ProjectileClass;

    // 디버그 라인 표시
    UPROPERTY(EditAnywhere, Category="Debug")
    bool bDrawSightDebug = true;

private:
    TObjectPtr<APawn> CandidatePlayer;  // Sphere로 들어온 후보
    TObjectPtr<APawn> CachedPlayer;     // 최종 타겟(보이는 상태에서만)

    float SightCosThreshold = 0.f;

    ETurretState State = ETurretState::Idle;

    // 초기 회전(감지 종료 시 복귀)
    FRotator InitialRotation;

    // Timers
    FTimerHandle FireTimerHandle;
    FTimerHandle OpenAnimTimerHandle;
    FTimerHandle CloseAnimTimerHandle;

};
