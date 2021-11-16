// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnemyState.h"

#include "DrawDebugHelpers.h"

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

	APlayerCharacter* me = Cast<APlayerCharacter>(GetPawn());
	me->GetCharacterMovement()->MaxWalkSpeed = 400.f;

	RunBehaviorTree(btree);
	behaviorTreeComponent->StartTree(*btree);

	blackboard->SetValueAsVector("movement", FVector::ZeroVector);

	blackboard->SetValueAsFloat("attackRange", 75.f);
	blackboard->SetValueAsFloat("acceptableRadius", 300.f);

	blackboard->SetValueAsEnum("state", (uint8)EnemyState::INPATROL);
}

void ACustomAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCharacter* me = Cast<APlayerCharacter>(GetPawn());
	blackboard->SetValueAsString("Debug", me->attacking ? "ATTACKING" : "NOT ATTACKING");
	blackboard->SetValueAsInt("health", me->health);

	FVector playerPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	blackboard->SetValueAsVector("targetPos", playerPos);

	FVector dirPlayerToMe = me->GetActorLocation() - playerPos;
	dirPlayerToMe.Normalize();
	FVector safePos = playerPos + dirPlayerToMe * blackboard->GetValueAsFloat("acceptableRadius");
	blackboard->SetValueAsVector("safePos", safePos);

	blackboard->SetValueAsBool("dead", me->dead);
	blackboard->SetValueAsBool("canSeeTarget", me->canSeeTarget);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), me->canSeeTarget));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%d"), blackboard->GetValueAsBool("canSeeTarget")));

	//DrawDebugSphere(GetWorld(), safePos, 30.f, 16, FColor::Green, false, 0.1f);
}