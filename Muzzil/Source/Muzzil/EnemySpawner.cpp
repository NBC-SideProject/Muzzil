#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBase.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = SpawnArea;
	SpawnArea->SetBoxExtent(FVector(500.f, 500.f, 100.f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpawnCount = 20;
	DefeatedCount = 0;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnWave();
}

void AEnemySpawner::SpawnWave()
{
	if (EnemyClasses.Num() == 0) return;

	DefeatedCount = 0;

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, EnemyClasses.Num() - 1);
		TSubclassOf<AEnemyBase> SelectedClass = EnemyClasses[RandomIndex];

		if (SelectedClass)
		{
			FVector SpawnOrigin = SpawnArea->GetComponentLocation();
			FVector SpawnExtent = SpawnArea->GetScaledBoxExtent();
			FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
			RandomLocation.Z = SpawnOrigin.Z;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(SelectedClass, RandomLocation, GetActorRotation(), SpawnParams);

			if (SpawnedEnemy)
			{
				SpawnedEnemy->OnDestroyed.AddDynamic(this, &AEnemySpawner::OnEnemyDestroyed);
			}
		}
	}
}

void AEnemySpawner::OnEnemyDestroyed(AActor* DestroyedActor)
{
	DefeatedCount++;
	UE_LOG(LogTemp, Warning, TEXT("처치한 적: %d / %d"), DefeatedCount, SpawnCount);

	if (DefeatedCount >= SpawnCount)
	{
		SpawnBoss();
	}
}

void AEnemySpawner::SpawnBoss()
{
	if (BossClasses.Num() == 0) return;

	int32 RandomIndex = FMath::RandRange(0, BossClasses.Num() - 1);
	TSubclassOf<AEnemyBase> SelectedBoss = BossClasses[RandomIndex];

	if (SelectedBoss)
	{
		FVector BossSpawnLocation = SpawnArea->GetComponentLocation();
		BossSpawnLocation.Z += 100.0f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		GetWorld()->SpawnActor<AEnemyBase>(SelectedBoss, BossSpawnLocation, GetActorRotation(), SpawnParams);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("경고! 보스가 등장했습니다!"));
	}
}