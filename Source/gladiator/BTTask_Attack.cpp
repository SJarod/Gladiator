// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* cont = OwnerComp.GetAIOwner();

	FString debug = cont->GetBlackboardComponent()->GetValueAsString(GetSelectedBlackboardKey());
	return EBTNodeResult::Succeeded;
}