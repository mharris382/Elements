// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ElementSubsystem.h"
#include "ElementsAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FReceivedDamageDelegate, UElementsAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage, EElementRelationship, DamageRelationship);
/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UElementsAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	UPROPERTY(BlueprintAssignable, Category = "Elements|Damage")
	FReceivedDamageDelegate OnReceivedDamage;

	// Called from ElementDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UElementsAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage, EElementRelationship DamageRelationship);
};
