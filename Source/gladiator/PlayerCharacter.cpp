// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

void APlayerCharacter::MoveForward(float value)
{
	if (dead || playAttack || playBlock)
		return;

	PlayForward(value * speed);

	if (!Controller || (value == 0.f))
		return;

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, value * speed);
}

void APlayerCharacter::MoveRight(float value)
{
	if (dead || playAttack || playBlock)
		return;

	PlayRight(value * speed);

	if (!Controller || (value == 0.f))
		return;

	// find out which way is right
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value * speed);
}

void APlayerCharacter::setMtlBlink(bool activate)
{
	if (activate)
	{
		float curTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		GetMesh()->SetScalarParameterValueOnMaterials("Activate", 1.f);
		GetMesh()->SetScalarParameterValueOnMaterials("StartTime", curTime);
	}
	else
	{
		GetMesh()->SetScalarParameterValueOnMaterials("Activate", 0.f);
	}
}

void APlayerCharacter::setMtlBlinkFalse()
{
	if (health != 1)
		setMtlBlink(false);
}

void APlayerCharacter::Die()
{
	dead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	hammerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	shieldCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::PlayForward(float value)
{
	FBSpeed = value;
}

void APlayerCharacter::PlayRight(float value)
{
	LRSpeed = value;
}

void APlayerCharacter::Jump()
{
	if (dead || !Controller || playAttack || playBlock)
		return;

	ACharacter::Jump();
}

void APlayerCharacter::ViewZoom(float value)
{
	cameraBoom->TargetArmLength += value * zoomSpeed;
}

void APlayerCharacter::Attack()
{
	if (dead || playBlock || playAttack || !Controller)
		return;

	playAttack = true;

	GetWorldTimerManager().ClearTimer(attackTimer);
	GetWorldTimerManager().SetTimer(attackTimer, this, &APlayerCharacter::EndAttack, attackTimeRate, false);
}

void APlayerCharacter::EndAttack()
{
	playAttack = false;
}

void APlayerCharacter::Block()
{
	if (dead)
		return;

	EndAttack();
	playBlock = true;
}

void APlayerCharacter::Unblock()
{
	playBlock = false;
}

void APlayerCharacter::TakeDamage()
{
	--health;
	healthPerCent = (float)health / (float)maxHealth;
	OnHealthUpdate.Broadcast();

	setMtlBlink(true);

	GetWorldTimerManager().ClearTimer(dmgTimer);
	GetWorldTimerManager().SetTimer(dmgTimer, this, &APlayerCharacter::setMtlBlinkFalse, dmgBlinkTimeRate, false);

	if (health <= 0)
		Die();
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
	shieldCollider->SetRelativeLocation(FVector(0.f, 9.f, -5.f));
	shieldCollider->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	shieldCollider->SetWorldScale3D(FVector(1.f, 0.1f, 1.f));

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

	//static ConstructorHelpers::FObjectFinder<UAnimBlueprint> animbp(TEXT("/Game/Characters/DwarfGrunt/Blueprint/PlayerDwarfAnimationBP.PlayerDwarfAnimationBP"));
	//GetMesh()->SetAnimInstanceClass(animbp.Object->GeneratedClass);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> tempList;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerCharacter::StaticClass(), TEXT("Player"), tempList);
	if (tempList.Num() > 0)
		target = Cast<APlayerCharacter>(tempList[0]);

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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("ViewYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("ViewPitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("ViewZoom", this, &APlayerCharacter::ViewZoom);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &APlayerCharacter::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &APlayerCharacter::Unblock);
}

void APlayerCharacter::OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* dwarfCast = Cast<APlayerCharacter>(OtherActor);
	if (attacking)
	{
		FString colliderName = UKismetSystemLibrary::GetObjectName(OtherComp);
		if (colliderName == "shield collider" && dwarfCast->playBlock)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, colliderName);
			attacking = false;
		}
		else if (colliderName == "CollisionCylinder" && Tags[0] != OtherActor->Tags[0])
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, colliderName);
			dwarfCast->TakeDamage();
		}
	}
}

void APlayerCharacter::UpdateSeeTarget()
{
	//GetController()->LineOfSightTo(Cast<AActor>(target), (FVector)(ForceInit), canSeeTarget);
	GetController()->LineOfSightTo(Cast<AActor>(target), GetActorLocation(), canSeeTarget);
}