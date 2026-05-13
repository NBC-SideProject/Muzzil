// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SparrowMesh(TEXT("/Game/Assets/Sparrow/Sparrow_LOD0.Sparrow_LOD0"));
	if (SparrowMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SparrowMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> SparrowWalkAnimation(TEXT("/Game/Assets/Sparrow/Anim/Sparrow_Walk.Sparrow_Walk"));
	if (SparrowWalkAnimation.Succeeded())
	{
		IdleWalkAnimation = SparrowWalkAnimation.Object;
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->SetAnimation(IdleWalkAnimation);
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContext(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (InputMappingContext.Succeeded())
	{
		DefaultMappingContext = InputMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputAction(TEXT("/Game/Input/Action/IA_ChangeDirection.IA_ChangeDirection"));
	if (InputAction.Succeeded())
	{
		ChangeDirectionAction = InputAction.Object;
	}

	ApplyFacingDirection();
}

FVector APlayerCharacter::GetPlayerBoundary()
{
	FVector Loc = PlayerBoundaryBoxComponent->GetComponentLocation();
	FVector BoxSize = PlayerBoundaryBoxComponent->GetScaledBoxExtent();
	
	return FVector();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (DefaultMappingContext)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (IdleWalkAnimation)
	{
		GetMesh()->PlayAnimation(IdleWalkAnimation, true);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(FVector::RightVector, static_cast<float>(PlayerDirection) * AutoMoveScale);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ChangeDirectionAction)
		{
			EnhancedInputComponent->BindAction(ChangeDirectionAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeDirection);
		}
	}
}

void APlayerCharacter::ChangeDirection(const FInputActionValue& Value)
{
	PlayerDirection *= -1;
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
	}
	ApplyFacingDirection();
}

void APlayerCharacter::ApplyFacingDirection()
{
	const float FacingYaw = PlayerDirection > 0 ? 0.0f : 180.0f;
	SetActorRotation(FRotator(0.0f, FacingYaw, 0.0f));
}

