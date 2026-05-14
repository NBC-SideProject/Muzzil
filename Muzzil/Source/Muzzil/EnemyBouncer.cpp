#include "EnemyBouncer.h"
#include "TimerManager.h" 
#include "GameFramework/CharacterMovementComponent.h" // 무브먼트 컴포넌트 사용을 위해 추가

AEnemyBouncer::AEnemyBouncer()
{
	JumpInterval = 2.0f;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->bOrientRotationToMovement = true;

		MovementComp->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	}
}	

void AEnemyBouncer::BeginPlay()
{
	Super::BeginPlay();

	// 💡 부모(EnemyBase)에 적어둔 MoveSpeed를 캐릭터의 실제 걷기 속도에 적용합니다.
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->MaxWalkSpeed = MoveSpeed;
	}

	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &AEnemyBouncer::DoBouncerJump, JumpInterval, true, 0.1f);
}

void AEnemyBouncer::DoBouncerJump()
{
	// 💡 이제 시스템이 충돌하지 않으므로, 순정 Jump()가 완벽하게 작동합니다!
	Jump();
}

void AEnemyBouncer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 💡 핵심: 강제 좌표 이동(AddActorWorldOffset)을 버리고, 정석 이동 방식을 씁니다.
	// 알아서 중력과 점프를 유지하면서 부드럽게 걸어갑니다!
	AddMovementInput(MoveDirection, 1.0f);
}

// 💡 튕김 로직: 걷거나 뛰다가 벽(장애물)에 부딪히면 이 함수가 자동으로 실행됩니다.
void AEnemyBouncer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// 다른 적이랑 부딪힌 거면 튕기지 않고 무시
	if (Other && Other->IsA(AEnemyBase::StaticClass())) return;

	// 당구공처럼 입사각/반사각 계산
	MoveDirection = FMath::GetReflectionVector(MoveDirection, HitNormal);
	MoveDirection.Z = 0.0f;

	// 반사 방향으로 회전
	if (!MoveDirection.IsNearlyZero())
	{
		SetActorRotation(MoveDirection.Rotation());
	}
}