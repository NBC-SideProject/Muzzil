// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UBoxComponent;
struct FInputActionValue;

UCLASS()
class MUZZIL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(Category="Collision")
	TObjectPtr<UBoxComponent> PlayerBoundaryBoxComponent;
	
	
	
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
public:
	FVector GetPlayerBoundary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ChangeDirectionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimationAsset> IdleWalkAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AutoMoveScale = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	int32 PlayerDirection = 1;

	void ChangeDirection(const FInputActionValue& Value);

	void ApplyFacingDirection();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
