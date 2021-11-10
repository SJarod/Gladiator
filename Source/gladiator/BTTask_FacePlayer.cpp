// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FacePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PlayerCharacter.h"

UBTTask_FacePlayer::UBTTask_FacePlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Face Player");
}

EBTNodeResult::Type UBTTask_FacePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* cont = OwnerComp.GetAIOwner();
	APlayerCharacter* me = Cast<APlayerCharacter>(cont->GetPawn());

	if (me->dead)
		return EBTNodeResult::Failed;

	FVector playerPos = cont->GetBlackboardComponent()->GetValueAsVector("targetPos");
	FVector dir = playerPos - me->GetActorLocation();
	dir = { dir.X, dir.Y, 0.f };

	me->SetActorRotation(dir.ToOrientationQuat());
	return EBTNodeResult::Succeeded;
}