// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_FacePlayer.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FacePlayer::UBTService_FacePlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Face Player");
}

void UBTService_FacePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		if (npc->dead || npc->playAttack || npc->playBlock)
			return;

		FVector playerPos = cont->GetBlackboardComponent()->GetValueAsVector("targetPos");
		FVector dir = playerPos - npc->GetActorLocation();
		dir = { dir.X, dir.Y, 0.f };

		npc->SetActorRotation(dir.ToOrientationQuat());
	}
}