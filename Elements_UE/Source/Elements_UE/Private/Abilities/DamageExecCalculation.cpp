// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DamageExecCalculation.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/AttributeSetBase.h"
#include "ElementsBlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "ElementSubsystem.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct ElementsDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	ElementsDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Armor, Source, false);
	}
};

static const ElementsDamageStatics& DamageStatics()
{
	static ElementsDamageStatics DStatics;
	return DStatics;
}

UDamageExecCalculation::UDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();


	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: TargetASC is null"));
		return;
	}
	if(!SourceASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: SourceASC is null"));
		return;
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();


	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Multiplier = 1.0f;
	EElementRelationship Relationship = EElementRelationship::Neutral;// = UElementsBlueprintFunctionLibrary::GetElementRelationshipBetweenActors(SourceActor, TargetActor);
	UWorld* World = TargetASC->GetWorld();
	if (World) {
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			UElementSubsystem* ElementSubsystem = GameInstance->GetSubsystem<UElementSubsystem>();
			if (ElementSubsystem)
			{
				FGameplayTag SourceElementTag = ElementSubsystem->GetElementTag(*SourceTags);
				FGameplayTag TargetElementTag = ElementSubsystem->GetElementTag(*TargetTags);
				Relationship = ElementSubsystem->GetElementRelationship(SourceElementTag, TargetElementTag);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: ElementSubsystem is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: GameInstance is null"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: World is null"));
	}
	Multiplier = GetMultiplierForRelationship(Relationship);

	

	// Get the Source's Armor
	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, FAggregatorEvaluateParameters(), Armor);

	float Damage = 0.0f;

	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	const bool damageExecutionModifierFound = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	const float setByCallerDamage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f);

	Damage += FMath::Max<float>(setByCallerDamage, 0.0f);

	float UnmitigatedDamage = Damage * Multiplier; // Can multiply any damage boosters here

	//LOG THE NAME OF SourceActor{0}, NAME of TargetActor{1}, SourceTags{2}, TargetTags{3}, Armor{4}, UnmitigatedDamage{5}, Damage{6}, Multiplier{7}, damageExecutionModifierFound, and setByCallerDamage
	//format should be "Result Damage({5}) = Base Damage({6}) * Relationship({7}) | (modifier found={8})  SourceActorInfo:(SrcActor:{0}, SrcTags:{2}) | TargetActorInfo: (TargetActor:{1}, TargetTags: {3}, TargetArmor: {4})

	UE_LOG(LogTemp, Log, TEXT("Result Damage(%f) = Base Damage(%f) * Relationship(%f) | (modifier found=%s)  SourceActorInfo: (SrcActor: %s, SrcTags: %s) | TargetActorInfo: (TargetActor: %s, TargetTags: %s, TargetArmor: %f)"),
		UnmitigatedDamage,              // {5}
		Damage,                         // {6}
		Multiplier,                     // {7}
		damageExecutionModifierFound ? TEXT("true") : TEXT("false"),  // {8}
		*GetNameSafe(SourceActor),      // {0} (safe conversion to handle null actors)
		*SourceTags->ToString(),        // {2} (convert the FGameplayTagContainer to string)
		*GetNameSafe(TargetActor),      // {1}
		*TargetTags->ToString(),        // {3}
		Armor                           // {4}
	);

	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));


	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}


	// Broadcast damages to Target ASC
	UElementsAbilitySystemComponent* TargetElementsASC = Cast<UElementsAbilitySystemComponent>(TargetASC);
	if (TargetASC)
	{
		UE_LOG(LogTemp, Log, TEXT("UDamageExecCalculation::Execute_Implementation: Receiving Damage"));
		UElementsAbilitySystemComponent* SourceElementsASC = Cast<UElementsAbilitySystemComponent>(SourceASC);
		
		TargetElementsASC->ReceiveDamage(SourceElementsASC, UnmitigatedDamage, MitigatedDamage, Relationship);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::Execute_Implementation: TargetElementsASC is null"));
	}
	//// Set the Target's Health attribute to be the current value minus the damage
	//float NewHealth = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, FAggregatorEvaluateParameters(), NewHealth);
	//NewHealth -= Damage;

	//// Clamp the new health
	//float MaxHealth = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MaxHealthDef, FAggregatorEvaluateParameters(), MaxHealth);
	//NewHealth = FMath::Clamp<float>(NewHealth, 0.f, MaxHealth);

	//// Set the Target's Health attribute
	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Override, NewHealth));

	//// Broadcast health change event
	//UAttributeSetBase* TargetAttributeSet = TargetASC->GetSet<UAttributeSetBase>();
	//TargetAttributeSet->Health.SetBaseValue(NewHealth);
	//TargetAttributeSet->OnHealthChange.Broadcast(NewHealth, MaxHealth);
}

float UDamageExecCalculation::GetMultiplierForRelationship(EElementRelationship Relationship) const
{
	switch (Relationship)
	{
		case EElementRelationship::Neutral:
			return 1.0f;
		case EElementRelationship::Strong:
			return 2.0f;
		case EElementRelationship::Weak:
			return 0.5f;
	}
	UE_LOG(LogTemp, Warning, TEXT("UDamageExecCalculation::GetMultiplierForRelationship: Invalid Element Relationship"));
	return 1.0f;
}
