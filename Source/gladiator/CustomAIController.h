// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CustomAIController.generated.h"

UCLASS()
class GLADIATOR_API ACustomAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* behaviorTreeComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;

public:
	class UBlackboardComponent* getBB() const;

	ACustomAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void OnPossess(APawn* InPawn) override;
};