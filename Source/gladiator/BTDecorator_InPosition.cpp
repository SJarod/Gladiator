// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_InPosition.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_InPosition::UBTDecorator_InPosition(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Am I In Position");
}

bool UBTDecorator_InPosition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		FVector safePos = cont->GetBlackboardComponent()->GetValueAsVector("safePos");
		float distance = (safePos - npc->GetActorLocation()).Size();

		return distance > cont->GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());
	}

	return false;
}