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

	void moveForward(float value);
	void moveRight(float value);
	void viewZoom(float value);

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};