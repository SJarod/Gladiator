// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Attack.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Attack::UBTService_Attack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Attack Service");
}

void UBTService_Attack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (const APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		cont->GetBlackboardComponent()->SetValueAsString(GetSelectedBlackboardKey(), "TICK FROM SERVICE");
	}
}