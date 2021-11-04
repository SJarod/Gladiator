// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class GLADIATOR_API APlayerPawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent*	capsule;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent*		skmesh;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* 	skhammer;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* 	skshield;

	UPROPERTY(EditAnywhere)
	float speed = 1.f;
	UPROPERTY(EditAnywhere)
	float jumpForce = 1.f;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent*		cameraBoom;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent*			camera;
	UPROPERTY(VisibleAnywhere)
	class UPawnMovementComponent*	floatingPawnMovement;

	void jump();
	void moveForward(float value);
	void moveRight(float value);

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};