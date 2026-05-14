	#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h" // FHitResult 구조체를 UHT가 인식하도록 포함
#include "EnemyProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UPrimitiveComponent; // OnHit 함수의 매개변수를 위한 전방 선언

UCLASS()
class MUZZIL_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()

public:
	AEnemyProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	// 이제 UHT가 UPrimitiveComponent와 FHitResult를 완벽하게 인식합니다
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};