#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyStraight.generated.h"

UCLASS()
class MUZZIL_API AEnemyStraight : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyStraight();

protected:
	// 💡 여기서 BeginPlay와 Tick을 쓰겠다고 선언해 주어야 에러가 사라집니다!
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};