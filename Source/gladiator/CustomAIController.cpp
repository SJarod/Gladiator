// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UBlackboardComponent* ACustomAIController::getBB() const
{
	return blackboard;
}

ACustomAIController::ACustomAIController(const FObjectInitializer& ObjectInitializer)
	: AAIController(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Game/AI/CustomBehaviorTree"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("Behavior Tree Component"));
	blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));

	SetActorTickEnabled(true);
}

void ACustomAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behaviorTreeComponent->StartTree(*btree);
}

void ACustomAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCharacter* me = Cast<APlayerCharacter>(GetPawn());
	blackboard->SetValueAsString("Debug", me->attacking ? "ATTACKING" : "NOT ATTACKING");
	blackboard->SetValueAsInt("health", me->health);

	FVector playerPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	blackboard->SetValueAsVector("targetPos", playerPos);
}