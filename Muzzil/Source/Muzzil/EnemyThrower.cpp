#include "EnemyThrower.h"
#include "EnemyProjectile.h"
#include "TimerManager.h" // 타이머 사용을 위해 필요

AEnemyThrower::AEnemyThrower()
{
	PrimaryActorTick.bCanEverTick = true;
	ThrowInterval = 3.0f; // 기본적으로 3초마다 던짐
}

void AEnemyThrower::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작과 동시에 타이머 가동 (ThrowInterval 초마다 ThrowProjectile 함수를 반복 호출)
	GetWorld()->GetTimerManager().SetTimer(ThrowTimerHandle, this, &AEnemyThrower::ThrowProjectile, ThrowInterval, true);
}

void AEnemyThrower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector DeltaLocation = MoveDirection.GetSafeNormal() * MoveSpeed * DeltaTime;
	FHitResult HitResult;

	AddActorWorldOffset(DeltaLocation, true, &HitResult);

	if (HitResult.bBlockingHit)
	{
		//만약 부딪힌 대상이 '다른 적'이라면 튕기지 않고 그냥 무시합니다.
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(AEnemyBase::StaticClass()))
		{
			return;
		}

		// 벽에 끼이는 것을 방지하기 위해 튕겨져 나온 방향으로 살짝 밀어줍니다.
		AddActorWorldOffset(HitResult.ImpactNormal * 2.0f);

		// 이동 방향을 정반대로 바꿉니다.
		MoveDirection = -MoveDirection;
		MoveDirection.Z = 0.0f;

		// 핵심 수정: 절대 방향(Rotation)을 구하지 않고, 현재 회전값에서 180도만 더합니다!
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += 180.0f;
		SetActorRotation(NewRotation);
	}
}

void AEnemyThrower::ThrowProjectile()
{
	// 에디터에서 투사체 클래스를 지정하지 않았다면 예외 처리
	if (!ProjectileClass) return;

	UWorld* World = GetWorld();
	if (World)
	{
		// 1. 현재 이동 중인 방향 벡터를 가져옵니다. (길이를 1로 정규화)
		FVector CurrentDir = MoveDirection.GetSafeNormal();

		// 2. 스폰 위치 계산: 현재 이동하는 방향으로 50만큼 앞, 위로 50만큼 띄웁니다.
		FVector SpawnLocation = GetActorLocation() + (CurrentDir * 50.0f) + FVector(0.f, 0.f, 50.f);

		// 3. 이동 방향 벡터(FVector)를 언리얼의 회전값(FRotator)으로 변환합니다!
		FRotator SpawnRotation = CurrentDir.Rotation();

		// 4. 변환된 회전값에서 위로 45도(Pitch) 들어 올려 포물선을 그립니다.
		SpawnRotation.Pitch += 45.0f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		// 투사체 스폰!
		World->SpawnActor<AEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}