// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:

	AAIControllerBase();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

};
