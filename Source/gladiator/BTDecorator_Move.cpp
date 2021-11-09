// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_Move.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_Move::UBTDecorator_Move(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Check Acceptable Radius");
}

bool UBTDecorator_Move::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	if (const APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		FVector playerPos = cont->GetBlackboardComponent()->GetValueAsVector("targetPos");
		float distance = (playerPos - npc->GetActorLocation()).Size();

		bool shouldMove = distance > cont->GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());
		//whether or not the character should rotate to its movement
		npc->GetCharacterMovement()->bOrientRotationToMovement = shouldMove;
		return shouldMove;
	}

	return false;
}