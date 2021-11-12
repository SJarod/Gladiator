// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_InPosition.generated.h"

UCLASS()
class GLADIATOR_API UBTDecorator_InPosition : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_InPosition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};