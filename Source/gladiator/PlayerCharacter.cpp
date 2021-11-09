// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Components/TimelineComponent.h"

void APlayerCharacter::moveForward(float value)
{
	if (playAttack || playBlock)
		return;

	FBSpeed = value * speed;

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
	if (playAttack || playBlock)
		return;

	LRSpeed = value * speed;

	if (!Controller || (value == 0.f))
		return;

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value * speed);
}

void APlayerCharacter::jump()
{
	if (!Controller || playAttack || playBlock)
		return;

	Jump();
}

void APlayerCharacter::viewZoom(float value)
{
	cameraBoom->TargetArmLength += value * zoomSpeed;
}

void APlayerCharacter::attack()
{
	if (playBlock || playAttack || !Controller)
		return;

	playAttack = true;

	GetWorldTimerManager().ClearTimer(timeHandle);
	GetWorldTimerManager().SetTimer(timeHandle, this, &APlayerCharacter::endAttack, attackTimeRate, false);
}

void APlayerCharacter::endAttack()
{
	playAttack = false;
}

void APlayerCharacter::block()
{
	endAttack();
	playBlock = true;
}

void APlayerCharacter::unblock()
{
	playBlock = false;
}

void APlayerCharacter::takeDamage()
{
	--health;

	if (health <= 0)
		dead = true;
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character not to call Tick() every frame. You can turn this on to call Tick() every frame but lose performance if you need it.
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

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	cameraBoom->ProbeChannel = ECC_Visibility;

	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	camera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	camera->SetActive(true);
	camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> animbp(TEXT("/Game/Characters/DwarfGrunt/Blueprint/PlayerDwarfAnimationBP.PlayerDwarfAnimationBP"));
	GetMesh()->SetAnimInstanceClass(animbp.Object->GeneratedClass);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	hammerCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnHammerBeginOverlap);

	GetCharacterMovement()->JumpZVelocity = jumpForce;
	GetCharacterMovement()->AirControl = airControl;

	health = maxHealth;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::moveRight);

	PlayerInputComponent->BindAxis("ViewYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ViewPitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("ViewZoom", this, &APlayerCharacter::viewZoom);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::unblock);
}

void APlayerCharacter::OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* dwarfCast = Cast<APlayerCharacter>(OtherActor);
	if (attacking)
	{
		FString colliderName = UKismetSystemLibrary::GetObjectName(OtherComp);
		if (colliderName == "shield collider" && dwarfCast->IsBlocking())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, colliderName);
			attacking = false;
			return;
		}
		else if (colliderName == "CollisionCylinder")
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, colliderName);
			dwarfCast->TakeDamage();
		}
	}
}
void APlayerCharacter::TakeDamage()
{
	health -= 1;
	if (health == 0)
	{
		dead = true;
	}
}

bool APlayerCharacter::IsBlocking()
{
	return playBlock;
}