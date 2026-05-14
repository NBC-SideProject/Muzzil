// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "RoundManager.generated.h"

class APlayerCharacter;
class UBoxComponent;

UCLASS()
class MUZZIL_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Round")
	TObjectPtr<UBoxComponent> StageBoundary;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Round")
	TObjectPtr<UBoxComponent> PlayerBoundary;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Spawn")
	TArray<TSubclassOf<AActor>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Spawn", meta = (ClampMin = "0"))
	int32 EnemySpawnCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Spawn", meta = (ClampMin = "0.0"))
	float EnemySpawnInterval = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Stage")
	TArray<TObjectPtr<AActor>> StageMaps;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Stage")
	FVector StageStride = FVector(3000.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Round|Stage")
	int32 CurrentMapIndex = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	// 스테이지 위치
	FVector GetStageLocation();
	
	// 스테이지 크기
	FVector GetStageExtent();

	// 플레이어 위치
	FVector GetPlayerLocation();
	
	// 플레이어의 바운더리 크기 가져오기
	FVector GetPlayerBoundaryExtent();

protected:
	UFUNCTION()
	void OnStageBoundaryBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void StartRound(APlayerCharacter* PlayerCharacter);
	void SpawnNextEnemy();
	void FinishRoundSpawn();
	void AdvanceStage();
	FVector GetRandomSpawnLocation() const;

private:
	FTimerHandle EnemySpawnTimerHandle;
	TWeakObjectPtr<APlayerCharacter> ActivePlayer;
	FVector RoundManagerLocalOffset = FVector::ZeroVector;
	int32 NextMapIndex = 1;
	int32 SpawnedEnemyCount = 0;
	bool bRoundActive = false;
};
