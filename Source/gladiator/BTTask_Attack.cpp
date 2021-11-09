// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"

#include "AIController.h"
#include "PlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	APlayerCharacter* me = Cast<APlayerCharacter>(cont->GetPawn());

	me->attack();
	return EBTNodeResult::Succeeded;
}