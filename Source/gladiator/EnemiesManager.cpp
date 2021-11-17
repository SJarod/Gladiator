// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemiesManager.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyState.h"

#include <vector>

// Sets default values
AEnemiesManager::AEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemiesManager::BeginPlay()
{
	timeBetweenAttacks = maxTimeBetweenAttacks;
	Super::BeginPlay();
	TArray<AActor*> tempList;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerCharacter::StaticClass(), TEXT("Enemy"), tempList);
	
	for (AActor* enemy : tempList)
	{
		enemies.Add(Cast<APlayerCharacter>(enemy));
	}
}

// Called every frame
void AEnemiesManager::Tick(float DeltaTime)
{
	bool allEnemiesdead = true;

	Super::Tick(DeltaTime);
	timeBetweenAttacks -= DeltaTime;
	if (timeBetweenAttacks <= 0)
	{
		timeBetweenAttacks = maxTimeBetweenAttacks;
		EnemyAttack();
	}

	for (APlayerCharacter* enemy : enemies)
	{
		enemy->UpdateSeeTarget();

		if (!enemy->dead)
			allEnemiesdead = false;
	}

	if(allEnemiesdead)
		gameOver = true;
}

void AEnemiesManager::EnemyAttack()
{
	//check which enemy is INPOSITION (ready to fight)
	int index = 0;
	std::vector<int> validIndexes;
	for (APlayerCharacter* enemy : enemies)
	{
		UBlackboardComponent* bb = UAIBlueprintHelperLibrary::GetBlackboard(enemy);
		uint8 state = bb->GetValueAsEnum("state");

		if (state == (uint8)EnemyState::INPOSITION && !enemy->dead)
			validIndexes.push_back(index);

		++index;
	}

	if ((int)validIndexes.size() == 0)
		return;

	int randIndex = validIndexes[FMath::RandRange(0, (int)validIndexes.size() - 1)];
	APlayerCharacter* enemy = enemies[randIndex];

	UBlackboardComponent* blackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(enemy);
	blackboardComponent->SetValueAsEnum("state", (uint8)EnemyState::INFIGHT);
}