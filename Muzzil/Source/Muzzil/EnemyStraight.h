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

public:
    virtual void Tick(float DeltaTime) override;

};