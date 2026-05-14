// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundManager.h"

#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "PlayerCharacter.h"
#include "TimerManager.h"

// Sets default values
ARoundManager::ARoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StageBoundary = CreateDefaultSubobject<UBoxComponent>(TEXT("StageBoundary"));
	SetRootComponent(StageBoundary);
	StageBoundary->SetBoxExtent(FVector(900.0f, 240.0f, 360.0f));
	StageBoundary->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StageBoundary->SetCollisionResponseToAllChannels(ECR_Ignore);
	StageBoundary->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StageBoundary->SetGenerateOverlapEvents(true);

	PlayerBoundary = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerBoundary"));
	PlayerBoundary->SetupAttachment(StageBoundary);
	PlayerBoundary->SetBoxExtent(FVector(640.0f, 200.0f, 320.0f));
	PlayerBoundary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ARoundManager::BeginPlay()
{
	Super::BeginPlay();

	NextMapIndex = StageMaps.Num() > 1 ? (CurrentMapIndex + 1) % StageMaps.Num() : 0;

	if (StageMaps.IsValidIndex(CurrentMapIndex) && StageMaps[CurrentMapIndex])
	{
		RoundManagerLocalOffset = GetActorLocation() - StageMaps[CurrentMapIndex]->GetActorLocation();
	}

	StageBoundary->OnComponentBeginOverlap.AddDynamic(this, &ARoundManager::OnStageBoundaryBeginOverlap);
}

// Called every frame
void ARoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ARoundManager::GetStageLocation()
{
	return StageBoundary ? StageBoundary->GetComponentLocation() : GetActorLocation();
}

FVector ARoundManager::GetStageExtent()
{
	return StageBoundary ? StageBoundary->GetScaledBoxExtent() : FVector::ZeroVector;
}

FVector ARoundManager::GetPlayerLocation()
{
	return PlayerBoundary ? PlayerBoundary->GetComponentLocation() : GetActorLocation();
}

FVector ARoundManager::GetPlayerBoundaryExtent()
{
	return PlayerBoundary ? PlayerBoundary->GetScaledBoxExtent() : FVector::ZeroVector;
}

void ARoundManager::OnStageBoundaryBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (bRoundActive)
	{
		return;
	}

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		StartRound(PlayerCharacter);
	}
}

void ARoundManager::StartRound(APlayerCharacter* PlayerCharacter)
{
	bRoundActive = true;
	ActivePlayer = PlayerCharacter;
	SpawnedEnemyCount = 0;

	if (EnemySpawnCount <= 0 || EnemyClasses.IsEmpty())
	{
		FinishRoundSpawn();
		return;
	}

	SpawnNextEnemy();
}

void ARoundManager::SpawnNextEnemy()
{
	if (!GetWorld())
	{
		return;
	}

	if (SpawnedEnemyCount >= EnemySpawnCount)
	{
		FinishRoundSpawn();
		return;
	}

	TArray<TSubclassOf<AActor>> ValidEnemyClasses;
	for (const TSubclassOf<AActor>& EnemyClass : EnemyClasses)
	{
		if (EnemyClass)
		{
			ValidEnemyClasses.Add(EnemyClass);
		}
	}

	if (ValidEnemyClasses.IsEmpty())
	{
		FinishRoundSpawn();
		return;
	}

	const int32 EnemyClassIndex = FMath::RandRange(0, ValidEnemyClasses.Num() - 1);
	const FVector SpawnLocation = GetRandomSpawnLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	GetWorld()->SpawnActor<AActor>(ValidEnemyClasses[EnemyClassIndex], SpawnLocation, SpawnRotation, SpawnParams);

	++SpawnedEnemyCount;

	if (SpawnedEnemyCount >= EnemySpawnCount)
	{
		FinishRoundSpawn();
		return;
	}

	if (EnemySpawnInterval <= 0.0f)
	{
		SpawnNextEnemy();
		return;
	}

	GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, this, &ARoundManager::SpawnNextEnemy, EnemySpawnInterval, false);
}

void ARoundManager::FinishRoundSpawn()
{
	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(EnemySpawnTimerHandle);
	}

	AdvanceStage();
	bRoundActive = false;
}

void ARoundManager::AdvanceStage()
{
	if (StageMaps.Num() < 2 || !StageMaps.IsValidIndex(CurrentMapIndex) || !StageMaps.IsValidIndex(NextMapIndex))
	{
		SetActorLocation(GetActorLocation() + StageStride);
		return;
	}

	AActor* CurrentMap = StageMaps[CurrentMapIndex];
	AActor* NextMap = StageMaps[NextMapIndex];
	if (!CurrentMap || !NextMap)
	{
		SetActorLocation(GetActorLocation() + StageStride);
		return;
	}

	CurrentMap->SetActorLocation(NextMap->GetActorLocation() + StageStride);

	const int32 ReusedMapIndex = CurrentMapIndex;
	CurrentMapIndex = NextMapIndex;
	NextMapIndex = ReusedMapIndex;

	if (AActor* UpcomingMap = StageMaps[NextMapIndex])
	{
		SetActorLocation(UpcomingMap->GetActorLocation() + RoundManagerLocalOffset);
	}
}

FVector ARoundManager::GetRandomSpawnLocation() const
{
	if (!StageBoundary)
	{
		return GetActorLocation();
	}

	const FVector Origin = StageBoundary->GetComponentLocation();
	const FVector Extent = StageBoundary->GetScaledBoxExtent();

	return FVector(
		FMath::RandRange(Origin.X - Extent.X, Origin.X + Extent.X),
		FMath::RandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y),
		FMath::RandRange(Origin.Z - Extent.Z, Origin.Z + Extent.Z));
}

