// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GLADIATOR_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skhammer;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* skshield;

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

	//for the animation blueprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float FBSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float LRSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  playAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool  playBlock = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* camera;

	FTimerHandle timeHandle;
	UPROPERTY(EditAnywhere)
	float attackTimeRate = 0.5f;

	void moveForward(float value);
	void moveRight(float value);
	void jump();
	void viewZoom(float value);

	void attack();
	UFUNCTION()
	void endAttack();

	void block();
	void unblock();

	void takeDamage();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int  health = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool dead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool attacking = false;

	// Sets default values for this character's properties
	APlayerCharacter();

	UFUNCTION()
		void OnHammerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void TakeDamage();
	bool IsBlocking();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};