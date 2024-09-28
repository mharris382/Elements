// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Executions/ManaGainExecCalculation.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "Abilities/AttributeSets/AttributeSetBase.h"
#include "ElementsBlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "ElementSubsystem.h"


struct ManaGainStatics
{
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ManaGainBoost);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ManaGain);

	ManaGainStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ManaGainBoost, Source, false);
		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSet, ManaGain, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSetBase, Armor, Target, false);
	}
};

static const ManaGainStatics& MGStatics()
{
	static ManaGainStatics DStatics;
	return DStatics;
}


UManaGainExecCalculation::UManaGainExecCalculation()
{
	RelevantAttributesToCapture.Add(MGStatics().ManaGainDef);
	RelevantAttributesToCapture.Add(MGStatics().ManaGainBoostDef);
}

void UManaGainExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	/*if (!ExecutionParams.GetPassedInTags().IsValid() || ExecutionParams.GetPassedInTags().IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("UManaGainExecCalculation::Execute_Implementation: Mana Gain not given an elemenet tag!"));
		return;
	}*/

	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UManaGainExecCalculation::Execute_Implementation: TargetASC is null"));
		return;
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = &Spec.GetDynamicAssetTags();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Multiplier = 1.0f;
	FGameplayTag TargetElement = UElementsBlueprintFunctionLibrary::GetElementTag(TargetASC, *TargetTags);
	FGameplayTag ManaElement = UElementsBlueprintFunctionLibrary::GetElementTag(TargetASC, *SourceTags);
	if (TargetElement.MatchesTagExact(ManaElement))
	{
		Multiplier = 1.5f;
	}
	else 
	{
		Multiplier = 0.5f;
	}
	EElementRelationship Relationship = EElementRelationship::Neutral;
	


	float Boost = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MGStatics().ManaGainBoostDef, FAggregatorEvaluateParameters(), Boost);
	Boost /= 100.f;

	float ManaGain = 0.f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	const bool manaExecutionModifierFound = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MGStatics().ManaGainDef, EvaluationParameters, ManaGain);
	const float setByCallerMana = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ManaGain")), false, -1.0f);

	ManaGain += FMath::Max<float>(setByCallerMana, 0.0f);

	float TotalManaGain = (ManaGain * Multiplier) + ((Boost*Multiplier) / ManaGain);

	UE_LOG(LogTemp, Log, TEXT("TotalManaGain(%f) = ManaGain(%f) * Relationship(%f) | (modifier found=%s)  ManaPickupInfo: (SrcTags: %s) | TargetActorInfo: (TargetActor: %s, TargetTags: %s, TargetBoost: %f)"),
		TotalManaGain,              // {5}
		ManaGain,                         // {6}
		Multiplier,                     // {7}
		manaExecutionModifierFound ? TEXT("true") : TEXT("false"),  // {8}
		//*GetNameSafe(SourceActor),      // {0} (safe conversion to handle null actors)
		*SourceTags->ToString(),        // {2} (convert the FGameplayTagContainer to string)
		*GetNameSafe(TargetActor),      // {1}
		*TargetTags->ToString(),        // {3}
		Boost                           // {4}
	);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(MGStatics().ManaGainProperty, EGameplayModOp::Additive, TotalManaGain));
}

float UManaGainExecCalculation::GetMultiplierForRelationship(EElementRelationship Relationship) const
{
	return 0.0f;
}
