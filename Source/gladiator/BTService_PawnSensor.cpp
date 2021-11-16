// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_PawnSensor.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyState.h"

UBTService_PawnSensor::UBTService_PawnSensor(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Check Pawn Sensor");
}

void UBTService_PawnSensor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (const APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		if (npc->pawnSensorFoundPlayer)
			cont->GetBlackboardComponent()->SetValueAsEnum("state", (uint8)EnemyState::INPOSITION);
	}
}