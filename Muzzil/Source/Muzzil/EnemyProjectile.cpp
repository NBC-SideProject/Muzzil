#include "EnemyProjectile.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEnemyProjectile::AEnemyProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);

	CollisionComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

	RootComponent = CollisionComp;
	CollisionComp->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnHit);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 800.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.6f;
	ProjectileMovement->ProjectileGravityScale = 1.5f;
}

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(3.0f);
}