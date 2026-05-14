#include "EnemyStraight.h"
#include "GameFramework/CharacterMovementComponent.h" // 무브먼트 컴포넌트 사용을 위해 추가

AEnemyStraight::AEnemyStraight()
{
	PrimaryActorTick.bCanEverTick = true;
	MoveDirection = FVector(1.0f, 0.0f, 0.0f); // 기본 이동 방향 (X축)

	// 💡 스폰될 때 뇌(AI)를 장착해야 걸어오라는 명령을 듣습니다!
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 이동하는 방향을 바라보게 만드는 시선 처리 설정
	bUseControllerRotationYaw = false;
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->bOrientRotationToMovement = true;
		MovementComp->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	}
}

void AEnemyStraight::BeginPlay()
{
	Super::BeginPlay();

	// 부모 클래스의 MoveSpeed를 실제 걷기 속도에 적용
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->MaxWalkSpeed = MoveSpeed;
	}
}

void AEnemyStraight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 💡 정석 이동 명령: 알아서 부드럽게 걸어갑니다!
	AddMovementInput(MoveDirection, 1.0f);
}