// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

void APlayerCharacter::moveForward(float value)
{
	if (!Controller || (value == 0.f))
		return;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, value * speed);
}

void APlayerCharacter::moveRight(float value)
{
	if (!Controller || (value == 0.f))
		return;

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value * speed);
}

void APlayerCharacter::viewZoom(float value)
{
	cameraBoom->TargetArmLength += value * zoomSpeed;
}

void APlayerCharacter::attack()
{
	attacking = true;
	//timer
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("attacking"));
	attacking = true;
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//set weapons collider
	hammerCollider = CreateDefaultSubobject<UCapsuleComponent>("hammer collider");
	hammerCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	hammerCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	hammerCollider->CanCharacterStepUpOn = ECB_No;
	hammerCollider->SetCanEverAffectNavigation(false);
	hammerCollider->SetRelativeLocation(FVector(0.f, 57.f, 0.f));
	hammerCollider->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	hammerCollider->SetWorldScale3D(FVector(0.6f));

	shieldCollider = CreateDefaultSubobject<UBoxComponent>("shield collider");
	shieldCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	shieldCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	shieldCollider->CanCharacterStepUpOn = ECB_No;
	shieldCollider->SetCanEverAffectNavigation(false);
	shieldCollider->SetRelativeLocation(FVector(0.f, 15.f, -5.f));
	shieldCollider->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	shieldCollider->SetWorldScale3D(FVector(1.f, 0.28f, 1.f));

	//set weapons meshes
	skhammer = CreateDefaultSubobject<USkeletalMeshComponent>("hammer");
	skshield = CreateDefaultSubobject<USkeletalMeshComponent>("shield");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/DwarfGrunt_R_new"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> hammer(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/WarHammer"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> shield(TEXT("/Game/Characters/DwarfGrunt/SkelMesh/Shield_Skel"));

	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);

		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));
		GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.5f));

		GetCapsuleComponent()->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
		GetCapsuleComponent()->SetWorldScale3D(FVector(0.7f, 0.7f, 0.45f));
	}

	if (hammer.Succeeded())
	{
		skhammer->SetSkeletalMesh(hammer.Object);
	}

	if (shield.Succeeded())
	{
		skshield->SetSkeletalMesh(shield.Object);
	}

	skhammer->SetupAttachment(GetMesh(), FName(TEXT("WeaponPoint")));
	skshield->SetupAttachment(GetMesh(), FName(TEXT("DualWeaponPoint")));

	hammerCollider->SetupAttachment(skhammer);
	shieldCollider->SetupAttachment(skshield);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = jumpForce;
	GetCharacterMovement()->AirControl = airControl;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->SetActive(true);
	camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::moveRight);

	PlayerInputComponent->BindAxis("ViewYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ViewPitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("ViewZoom", this, &APlayerCharacter::viewZoom);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::attack);
}