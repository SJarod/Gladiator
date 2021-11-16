// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemiesManager.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyState.h"

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
	APlayerCharacter* enemy = enemies[FMath::RandRange(0, enemies.Num() - 1)];

	UBlackboardComponent* blackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(enemy);

	blackboardComponent->SetValueAsEnum("state", (uint8)EnemyState::INFIGHT);
}