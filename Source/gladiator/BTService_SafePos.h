// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_SafePos.generated.h"

UCLASS()
class GLADIATOR_API UBTService_SafePos : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_SafePos(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//replaces controller's tick to have better performance, only updates some variables
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};