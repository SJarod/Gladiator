// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyState.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	APlayerCharacter* me = Cast<APlayerCharacter>(cont->GetPawn());

	me->Attack();
	cont->GetBlackboardComponent()->SetValueAsEnum("state", (uint8)EnemyState::INPOSITION);
	return EBTNodeResult::Succeeded;
}