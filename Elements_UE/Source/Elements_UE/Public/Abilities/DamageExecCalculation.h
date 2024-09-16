// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ElementSubsystem.h"
#include "DamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;


private:
	float GetMultiplierForRelationship(EElementRelationship Relationship);
};
