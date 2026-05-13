#include "EnemyStraight.h"

AEnemyStraight::AEnemyStraight()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyStraight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector DeltaLocation = MoveDirection.GetSafeNormal() * MoveSpeed * DeltaTime;

    FHitResult HitResult;

    AddActorWorldOffset(DeltaLocation, true, &HitResult);

    if (HitResult.bBlockingHit)
    {
        MoveDirection = FMath::GetReflectionVector(MoveDirection, HitResult.ImpactNormal);

        MoveDirection.Z = 0.0f;
    }
}