// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/FloatingPawnMovement.h"

void APlayerPawn::jump()
{
	if (!Controller)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("jumping"));
}

void APlayerPawn::moveForward(float value)
{
	if (!Controller || (value == 0.f))
		return;

	FVector dir(1.f, 0.f, 0.f);
	AddMovementInput(dir, value);
}

void APlayerPawn::moveRight(float value)
{
	if (!Controller || (value == 0.f))
		return;

	FVector dir(0.f, 1.f, 0.f);
	AddMovementInput(dir, value);
}

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>("capsule collider");
	capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	capsule->CanCharacterStepUpOn = ECB_No;
	capsule->SetShouldUpdatePhysicsVolume(true);
	capsule->SetCanEverAffectNavigation(false);
	capsule->bDynamicObstacle = true;
	capsule->SetSimulatePhysics(true);
	capsule->BodyInstance.bLockXRotation = true;
	capsule->BodyInstance.bLockYRotation = true;
	RootComponent = capsule;

	skmesh = CreateDefaultSubobject<USkeletalMeshComponent>("skeletal mesh");
	skmesh->SetupAttachment(RootComponent);

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	cameraBoom->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->SetActive(true);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	floatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("floating pawn movement");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	//characterMovement->SetDefaultMovementMode();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::moveRight);
}