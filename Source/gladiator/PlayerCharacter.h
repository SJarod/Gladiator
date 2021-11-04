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
	UPROPERTY(EditAnywhere)
		float speed = 1.f;
	UPROPERTY(EditAnywhere)
		float jumpForce = 1.f;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* cameraBoom;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* camera;

	void moveForward(float value);
	void moveRight(float value);

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};