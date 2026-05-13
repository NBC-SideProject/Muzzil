	#include "EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(GetCapsuleComponent());
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	MoveSpeed = 300.0f;
	Damage = 10.0f;

	MoveDirection = FVector(-1.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

// Called every frame
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