// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"

class UBoxComponent;

UCLASS()
class MUZZIL_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

public:
	UPROPERTY()
	UBoxComponent* StageBoundary;
	
	UPROPERTY()
	UBoxComponent* PlayerBoundary;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	FVector GetStageLocation();
	FVector GetStageExtent();

	FVector GetPlayerLocation();
	FVector GetPlayerBoundaryExtent();
};
