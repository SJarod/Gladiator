// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_Attack.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_Attack::UBTDecorator_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Check Attack");
}

bool UBTDecorator_Attack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		return npc->attacking;
	}

	int health = cont->GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());
	return health == 5;
}