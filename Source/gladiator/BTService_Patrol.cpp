// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Patrol.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Patrol::UBTService_Patrol(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Random Patrol Position");
}

void UBTService_Patrol::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		//alpha is current position
		FVector alpha = npc->GetActorLocation();

		//beta is a vector that will be added to alpha to make a new position
		int dir = rand() % 2;
		dir == 0 ? dir = -1 : dir = 1;

		FVector beta = { (float)(rand() % 3 - 1), (float)(rand() % 3 - 1), 0.f };
		beta.Normalize();
		beta *= 250.f;

		cont->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), alpha + beta);
	}
}