// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllerBase.h"
#include "Characters/EnemyCharacter.h"

AAIControllerBase::AAIControllerBase()
{
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);
	if (EnemyCharacter && EnemyCharacter->BehaviorTree)
	{
		RunBehaviorTree(EnemyCharacter->BehaviorTree);
	}
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
}
