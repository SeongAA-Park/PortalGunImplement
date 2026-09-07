# PortalGunImplement — 프로젝트 기술 소개
<img width="496" height="321" alt="인게임 사진 1" src="https://github.com/user-attachments/assets/f92fa2ab-b4f1-478c-8f27-ffed52b2f75a" />

## 프로젝트 개요

Valve의 《Portal》에서 제공하는 핵심 플레이 기능을 Unreal Engine과 C++, 블루프린트로 재현한 게임 모작 프로젝트입니다. 포탈건으로 두 공간을 연결하고, 플레이어의 위치와 시선에 따라 각 포탈의 반대편 공간을 실시간으로 표시하며, 포탈을 통한 이동과 이동 속도 보존을 구현하는 데 중점을 두었습니다.

단순히 두 지점 사이를 이동하는 기능을 넘어, 서로 다른 방향으로 설치된 포탈 사이에서 위치와 회전을 변환하여 공간이 연결된 것처럼 보이도록 구성했습니다. 이 과정에서 언리얼의 액터·컴포넌트 구조, 충돌 및 트레이스 API, Scene Capture와 머티리얼, `FTransform`과 `FQuat` 등을 활용했습니다.

- 엔진: Unreal Engine 5.6 
- 구현 방식: C++ 및 블루프린트
- 핵심 범위: 포탈 생성, 실시간 포탈 시점 렌더링, 텔레포트, 텔레포트 시 플레이어 속도 보존

## 주요 기능과 구현 상태

| 기능 | 동작 | 현재 구현 |
| --- | --- | --- |
| 포탈건으로 포탈 생성 | 조준한 설치 가능 표면에 파란색·주황색 포탈을 생성하고 서로 연결 | C++ 중심 |
| 반대편 공간 실시간 표시 | 플레이어 카메라의 위치와 회전을 반대편 포탈 기준으로 변환하여 화면에 반영 | C++ 및 머티리얼 |
| 포탈 텔레포트 | 포탈 접근 시 플레이어를 반대편 포탈 쪽으로 이동 | 블루프린트, 추후 C++ 이전 예정 |
| 텔레포트 시 속도 보존 | 포탈 이동 전후에 플레이어의 이동 속도를 유지 | 블루프린트, 추후 C++ 이전 예정 |

## 1. 포탈건으로 포탈 생성

### 입력에서 포탈 배치까지

`APortalGunShooterCharacter`는 Enhanced Input으로 파란색·주황색 포탈 발사 입력을 처리합니다. 발사 입력은 포탈건의 `HandlePortalShot()`과 `ExecutePortalTrace()`로 이어지며, 캐릭터의 `GetWeaponTargetLocation()`에서 수행한 `LineTraceSingleByChannel()`의 충돌 결과를 사용합니다.

### 표면 방향에 맞는 회전 계산

생성 위치는 `ImpactPoint + ImpactNormal * 0.1f`로 계산합니다. 표면 법선 방향으로 조금 띄워 포탈 면과 벽이 겹칠 때 발생하는 Z-fighting을 줄이려는 처리입니다.

회전은 표면 법선을 포탈의 전방인 X축으로 삼고, 포탈의 위쪽인 Z축을 별도로 계산한 뒤 `UKismetMathLibrary::MakeRotFromXZ()`로 구성합니다.

## 2. 플레이어 시점을 반영한 실시간 포탈 렌더링

### 렌더링 구성

`ACustomPortal`은 다음 컴포넌트와 런타임 리소스를 사용합니다.

| 구성 요소 | 역할 |
| --- | --- |
| `USceneComponent` 루트 | 포탈 좌표계의 기준점 제공: 전방 +X, 위쪽 +Z |
| `UStaticMeshComponent` | 포탈 화면과 외형 표시 |
| `USceneCaptureComponent2D` | 반대편 공간을 변환된 카메라 시점으로 촬영 |
| `UTextureRenderTarget2D` | 촬영 결과를 텍스처로 저장 |
| `UMaterialInstanceDynamic` | 촬영 텍스처와 포탈 색상을 머티리얼에 전달 |
| `UBoxComponent` | 접근한 액터 감지 및 부착 벽 충돌 처리 |

포탈 메시의 외형 회전은 루트와 분리했습니다. 따라서 메시의 방향 보정이 포탈 좌표계의 기준이나 카메라 변환 계산에 영향을 주지 않도록 구성했습니다.

### Transform과 Quaternion을 이용한 시점 변환

핵심은 플레이어 카메라의 월드 좌표를 그대로 복사하는 대신, 입구 포탈에 대한 상대 위치와 회전을 구한 뒤 출구 포탈의 좌표계로 옮기는 것입니다. `UpdatePortalView3()`에서 다음 순서로 처리합니다.

1. `APlayerCameraManager`에서 플레이어 카메라의 위치와 회전을 읽습니다.
2. `GetRelativeTransform()`으로 현재 포탈 기준의 카메라 Transform을 구합니다.
3. 로컬 Z축을 기준으로 180도 회전하는 쿼터니언을 상대 위치와 회전에 적용합니다.
4. 변환된 상대 Transform을 연결된 포탈의 월드 Transform과 합성합니다.
5. 결과를 Scene Capture의 `SetWorldTransform()`에 전달합니다.

아래는 해당 구현의 핵심을 정리한 코드입니다.

```cpp
FTransform Relative = PlayerCamTransform.GetRelativeTransform(GetActorTransform());

FQuat FlipQuat(FVector(0, 0, 1), FMath::DegreesToRadians(180.f));
FVector RotatedLocation = FlipQuat.RotateVector(Relative.GetLocation());
FQuat RotatedRotation = FlipQuat * Relative.GetRotation();

FTransform RotatedRelative(RotatedRotation, RotatedLocation);
FTransform FinalTransform = RotatedRelative * LinkedPortal->GetActorTransform();
PortalCamera->SetWorldTransform(FinalTransform);
```

### 갱신 시점과 클리핑

포탈은 `TG_PostUpdateWork` Tick 그룹을 사용하며, `LinkedPortal`이 있을 때 매 프레임 시점 변환을 갱신합니다. Scene Capture에는 `bCaptureEveryFrame`과 `bCaptureOnMovement`가 활성화되어 있습니다.

## 3. 포탈 접근 시 텔레포트

플레이어가 포탈에 접근하면 연결된 반대편 포탈 쪽으로 이동하는 기능을 블루프린트 노드로 구현했습니다. 포탈 렌더링과 결합하여, 플레이어가 눈앞에 보이는 반대편 공간으로 실제 이동하는 상호작용을 제공합니다.

C++의 `ACustomPortal`에는 접근 감지와 부착 벽의 충돌 처리가 구현되어 있습니다. `DetectBoxCollision`은 Pawn과 PhysicsBody 채널의 Overlap을 감지하고, Pawn 또는 `PortalPassable` 태그가 있는 액터에 대해 벽 통과를 보조합니다.

- Movement 기반 액터는 `IgnoreActorWhenMoving()`으로 액터와 부착 벽 사이의 이동 충돌을 무시합니다.
- 물리 시뮬레이션 액터는 `PortalWall` 채널 응답을 Overlap으로 변경하고 `WakeAllRigidBodies()`를 호출합니다.
- Overlap 종료 시 이동 충돌 무시를 해제하거나 해당 채널을 Block으로 되돌립니다.

이 C++ 처리는 벽 통과를 위한 충돌 보조 로직입니다. 실제 플레이어 위치를 반대편 포탈로 이동시키는 텔레포트는 현재 블루프린트의 역할이며, 추후 C++로 이전할 예정입니다.

## 4. 텔레포트 시 플레이어 속도 보존

포탈 이동 전후에 플레이어의 속도를 보존하는 기능을 블루프린트로 구현했습니다. 이동 중 포탈에 진입했을 때 출구에서도 이동이 이어지도록 하여, 위치 이동과 함께 운동의 연속성을 표현하는 기능입니다.

이 기능은 텔레포트 로직과 함께 C++로 이전할 예정입니다.

## 구현을 통해 다룬 기술

- **좌표계 변환:** 월드 공간과 포탈 로컬 공간 사이의 `FTransform` 변환 및 합성.
- **회전과 벡터 연산:** `FQuat`을 이용한 위치 벡터 회전과 자세 합성, 내적과 평면 투영을 이용한 설치 방향 계산.
- **실시간 렌더링:** Scene Capture → Render Target → 동적 머티리얼로 이어지는 포탈 화면 구성.
- **언리얼 객체 구조:** 액터와 컴포넌트의 역할 분리, 지연 스폰, `TObjectPtr` 및 부착 벽 참조를 위한 `TWeakObjectPtr` 사용.
- **입력과 충돌:** Enhanced Input, 라인트레이스, Overlap 델리게이트와 충돌 채널 응답 제어.

## 후속 개발 계획

- 텔레포트와 속도 보존 로직을 블루프린트에서 C++로 이전 및 검토 
