// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

void APlayerPawn::jump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("jumping"));
}

void APlayerPawn::moveForward(float value)
{
	if (!Controller || (value == 0.f))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("moving forward"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString::SanitizeFloat(value));
}

void APlayerPawn::moveRight(float value)
{
	if (!Controller || (value == 0.f))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("moving right"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString::SanitizeFloat(value));
}

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>("capsule collider");
	RootComponent = capsule;

	skmesh = CreateDefaultSubobject<USkeletalMeshComponent>("skeletal mesh");
	skmesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/DwarfGrunt_R_new"));

	if (mesh.Succeeded())
	{
		skmesh->SetSkeletalMesh(mesh.Object);
		skmesh->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		skmesh->SetWorldScale3D(FVector(.7f));
		skmesh->SetWorldRotation(FRotator(0.f,-90.f,0.f));
	}

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	cameraBoom->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->SetActive(true);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	characterMovement = CreateDefaultSubobject<UCharacterMovementComponent>("character movement");
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerPawn::jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerPawn::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::moveRight);
}