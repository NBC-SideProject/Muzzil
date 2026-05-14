#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyThrower.generated.h"

UCLASS()
class MUZZIL_API AEnemyThrower : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyThrower();

protected:
	virtual void BeginPlay() override;

	// 어떤 투사체를 던질지 에디터에서 선택할 수 있게 해주는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	TSubclassOf<class AEnemyProjectile> ProjectileClass;

	// 공을 던지는 주기 (초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attack")
	float ThrowInterval;

	// 타이머를 관리할 핸들
	FTimerHandle ThrowTimerHandle;

	// 실제로 공을 스폰하는 함수
	void ThrowProjectile();

public:
	virtual void Tick(float DeltaTime) override;
};