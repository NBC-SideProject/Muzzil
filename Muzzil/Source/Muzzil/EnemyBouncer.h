#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBouncer.generated.h"

UCLASS()
class MUZZIL_API AEnemyBouncer : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyBouncer();

protected:
	virtual void BeginPlay() override;

	// 💡 벽에 부딪혔을 때 자동으로 호출되는 언리얼 내장 함수!
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	FTimerHandle JumpTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bouncer|Movement")
	float JumpInterval;

	void DoBouncerJump();

public:
	virtual void Tick(float DeltaTime) override;
};