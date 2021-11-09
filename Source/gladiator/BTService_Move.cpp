// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_Move.h"

#include "AIController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Move::UBTService_Move(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Update Blackboard Speed");
}

void UBTService_Move::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* cont = OwnerComp.GetAIOwner();
	if (APlayerCharacter* npc = Cast<APlayerCharacter>(cont->GetPawn()))
	{
		FVector worldSpeed = npc->GetVelocity();
		if (worldSpeed.Size() == 0.f)
		{
			cont->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FVector::ZeroVector);
			npc->playForward(0.f);
			npc->playRight(0.f);
			return;
		}

		const FRotator YawRotation(0, npc->GetActorRotation().Yaw, 0);
		const FVector forwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector rightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//get speed with the local referential
		float xSpeed = FVector::DotProduct(worldSpeed, forwardDir) / worldSpeed.Size();
		float ySpeed = FVector::DotProduct(worldSpeed, rightDir) / worldSpeed.Size();

		FVector localSpeed = { xSpeed, ySpeed, 0.f };
		cont->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), localSpeed);

		npc->playForward(xSpeed);
		npc->playRight(ySpeed);
	}
}