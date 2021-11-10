// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_False.generated.h"

UCLASS()
class GLADIATOR_API UBTTask_False : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_False(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};