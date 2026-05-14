// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1800.f;
	
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	MainCameraComponent->OrthoWidth = CameraOrthoWidth;
	
	MainCameraComponent->bUsePawnControlRotation = false;
	
	BackWallComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BackWall"));
	BackWallComponent->SetupAttachment(GetRootComponent());
	BackWallComponent->SetRelativeLocation(FVector(-260.0f, 0.0f, 0.0f));
	BackWallComponent->SetBoxExtent(FVector(24.0f, 160.0f, 220.0f));
	BackWallComponent->SetCollisionProfileName(TEXT("BlockAll"));

	BackWallComponent->SetUsingAbsoluteRotation(true);
	BackWallComponent->SetWorldRotation(FRotator::ZeroRotator);
	BackWallComponent->SetUsingAbsoluteLocation(false);
	BackWallComponent->SetUsingAbsoluteRotation(true);
	BackWallComponent->SetUsingAbsoluteScale(false);
	
	FrontTriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontTriggerBox"));
	FrontTriggerBoxComponent->SetupAttachment(GetRootComponent());
	FrontTriggerBoxComponent->SetRelativeLocation(FVector(420.0f, 0.0f, 0.0f));
	FrontTriggerBoxComponent->SetBoxExtent(FVector(64.0f, 160.0f, 220.0f));
	FrontTriggerBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FrontTriggerBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	FrontTriggerBoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	FrontTriggerBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FrontTriggerBoxComponent->SetUsingAbsoluteRotation(true);
	FrontTriggerBoxComponent->SetWorldRotation(FRotator::ZeroRotator);
	FrontTriggerBoxComponent->SetUsingAbsoluteLocation(false);
	FrontTriggerBoxComponent->SetUsingAbsoluteRotation(true);
	FrontTriggerBoxComponent->SetUsingAbsoluteScale(false);
	
	PlayerBoundaryBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerBoundaryBox"));
	PlayerBoundaryBoxComponent->SetupAttachment(GetRootComponent());
	PlayerBoundaryBoxComponent->SetBoxExtent(FVector(420.0f, 160.0f, 260.0f));
	PlayerBoundaryBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlayerBoundaryBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerBoundaryBoxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	PlayerBoundaryBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	PlayerBoundaryBoxComponent->SetUsingAbsoluteRotation(true);
	PlayerBoundaryBoxComponent->SetWorldRotation(FRotator::ZeroRotator);
	PlayerBoundaryBoxComponent->SetUsingAbsoluteLocation(false);
	PlayerBoundaryBoxComponent->SetUsingAbsoluteRotation(true);
	PlayerBoundaryBoxComponent->SetUsingAbsoluteScale(false);

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
}

FVector APlayerCharacter::GetPlayerBoundary()
{
	return GetPlayerBoundaryExtent();
}

FVector APlayerCharacter::GetPlayerBoundaryLocation() const
{
	return PlayerBoundaryBoxComponent ? PlayerBoundaryBoxComponent->GetComponentLocation() : GetActorLocation();
}

FVector APlayerCharacter::GetPlayerBoundaryExtent() const
{
	return PlayerBoundaryBoxComponent ? PlayerBoundaryBoxComponent->GetScaledBoxExtent() : FVector::ZeroVector;
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

	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		MeshBaseRelativeScale = SkeletalMeshComponent->GetRelativeScale3D();
		ApplyFacingDirection();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(FVector::ForwardVector, static_cast<float>(PlayerDirection) * AutoMoveScale);
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
	// ApplyFacingDirection();
}

void APlayerCharacter::ApplyFacingDirection()
{
	if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
	{
		FVector FacingScale = MeshBaseRelativeScale;
		FacingScale.X = FMath::Abs(MeshBaseRelativeScale.X) * (PlayerDirection > 0 ? -1.0f : 1.0f);
		// SkeletalMeshComponent->SetRelativeScale3D(FacingScale);
	}
}
