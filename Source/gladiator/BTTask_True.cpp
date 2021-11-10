// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_True.h"

UBTTask_True::UBTTask_True(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Return True");
}

EBTNodeResult::Type UBTTask_True::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}