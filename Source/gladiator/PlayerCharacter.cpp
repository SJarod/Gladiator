// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

void APlayerCharacter::moveForward(float value)
{
	if (!Controller || (value == 0.f))
		return;

	FVector dir(1.f, 0.f, 0.f);
	AddMovementInput(dir, value);
}

void APlayerCharacter::moveRight(float value)
{
	if (!Controller || (value == 0.f))
		return;

	FVector dir(0.f, 1.f, 0.f);
	AddMovementInput(dir, value);
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	cameraBoom->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->SetActive(true);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::moveRight);
}