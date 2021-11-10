// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_False.h"

UBTTask_False::UBTTask_False(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Return False");
}

EBTNodeResult::Type UBTTask_False::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Failed;
}