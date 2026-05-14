#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1);

	// 우리가 만든 Enemy 채널 무시
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	// 언리얼 기본 캐릭터 채널인 'Pawn' 끼리도 절대 부딪히지 않게 강제 무시!
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	// 혹시 모를 카메라 간섭 무시 (적들끼리 비비적거릴 때 카메라가 흔들리는 현상 방지)
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(GetCapsuleComponent());
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MoveSpeed = 300.0f;
	Damage = 10.0f;

	MoveDirection = FVector(0.0f, 1.0f, 0.0f);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		CurrentHealth -= ActualDamage;

		UE_LOG(LogTemp, Warning, TEXT("Enemy Took %f Damage! Current Health: %f"), ActualDamage, CurrentHealth);

		if (CurrentHealth <= 0.f)
		{
			Die();
		}
	}
	return ActualDamage;
}

void AEnemyBase::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Died!"));

	Destroy();	
}