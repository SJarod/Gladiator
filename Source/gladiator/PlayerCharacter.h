// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEvent);

UCLASS()
class GLADIATOR_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent*		skhammer;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent*		skshield;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent*	hammerCollider;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent*		shieldCollider;

	UPROPERTY(EditAnywhere)
	int   maxHealth = 5;
	UPROPERTY(EditAnywhere)
	float speed = 1.f;
	UPROPERTY(EditAnywhere)
	float jumpForce = 420.f;
	UPROPERTY(EditAnywhere)
	float airControl = 0.2f;
	UPROPERTY(EditAnywhere)
	float zoomSpeed = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* camera;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* pawnSensor;

	//time handles for timers
	FTimerHandle attackTimer;
	FTimerHandle dmgTimer;
	UPROPERTY(EditAnywhere)
	float attackTimeRate = 0.5f;
	UPROPERTY(EditAnywhere)
	float dmgBlinkTimeRate = 1.f;

	APlayerCharacter* target;

	void MoveForward(float value);
	void MoveRight(float value);

	void setMtlBlink(bool activate);
	void setMtlBlinkFalse();

	void Die();

public:
	//for the animation blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FBSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LRSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  playAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  playBlock = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int   health = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float healthPerCent = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool  dead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  attacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  pawnSensorFoundPlayer = false;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FHealthEvent OnHealthUpdate;

	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void PostInitializeComponents() override;

	//movement animations
	void PlayForward(float value);
	void PlayRight(float value);

	void Jump();
	void ViewZoom(float value);

	void Attack();
	UFUNCTION()
	void EndAttack();

	void Block();
	void Unblock();

	void TakeDamage();

	UFUNCTION()
	void OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool canSeeTarget = false;
	void UpdateSeeTarget();
};