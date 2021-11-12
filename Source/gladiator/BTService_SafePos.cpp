// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_SafePos.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "DrawDebugHelpers.h"

UBTService_SafePos::UBTService_SafePos(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Randomize Safe Pos");
}

void UBTService_SafePos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		//alpha is current safe position
		FVector alpha = cont->GetBlackboardComponent()->GetValueAsVector("safePos");

		const FRotator YawRotation(0, npc->GetActorRotation().Yaw, 0);
		const FVector rightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//beta is a vector that will be added to alpha to make a new position
		FVector beta = rightDir/* * (rand() % 2 - 1)*/;
		beta *= 100.f;

		cont->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), alpha + beta);

		DrawDebugSphere(GetWorld(), alpha + beta, 30.f, 16, FColor::Red, false, 0.1f);
	}
}