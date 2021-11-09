// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FacePlayer.generated.h"

UCLASS()
class GLADIATOR_API UBTTask_FacePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FacePlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};