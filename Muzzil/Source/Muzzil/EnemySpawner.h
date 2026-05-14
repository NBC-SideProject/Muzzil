#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class AEnemyBase;

UCLASS()
class MUZZIL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UBoxComponent* SpawnArea;

	// 일반 몬스터 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TArray<TSubclassOf<AEnemyBase>> EnemyClasses;

	// 👑 보스 몬스터 배열 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TArray<TSubclassOf<AEnemyBase>> BossClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 SpawnCount;

private:
	// 현재 처치한 적의 수
	int32 DefeatedCount;

	// 💡 적이 파괴될 때마다 자동으로 실행될 함수 (반드시 UFUNCTION이 붙어야 델리게이트로 쓸 수 있습니다)
	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	// 보스 소환 함수
	void SpawnBoss();

public:
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnWave();
};