// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Characters/CharacterBase.h"
#include "Player/ElementsPlayerController.h"
#include "Net/UnrealNetwork.h"

UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ACharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ACharacterBase>(TargetActor);
		if (!TargetCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAttributeSetBase::PostGameplayEffectExecute - Actor %s is not a valid TargetCharacter"), *GetNameSafe(TargetCharacter));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UAttributeSetBase::PostGameplayEffectExecute - No valid TargetActor or TargetController"));
	}



	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ACharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}
		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ACharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ACharacterBase>(SourceActor);
		}
		SourceCharacter = Cast<ACharacterBase>(SourceActor);

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}

		if (!SourceCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("UAttributeSetBase::PostGameplayEffectExecute - Actor %s is not a valid SourceCharacter"), *GetNameSafe(SourceCharacter));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UAttributeSetBase::PostGameplayEffectExecute - No valid TargetActor or SourceController"));
	}

	if (Data.EvaluatedData.Attribute == GetManaGainAttribute())
	{
		const float LocalManaGained = GetManaGain();
		SetManaGain(0.0f);
		if (LocalManaGained > 0)
		{
			// This is proper way to do it. We don't want to change the value of the attribute directly.
			// Instead, we want to use the SetMana function to make sure it's replicated properly.
			//SetMana(FMath::Clamp(GetMana() + LocalManaGained, 0.0f, GetMaxMana()));
		}
		const float NewMana = GetMana() + LocalManaGained;
		SetMana(FMath::Clamp(NewMana, 0.0f, GetMaxMana()));
		const float Overdraw = FMath::Max(0.0f, NewMana - GetMaxMana());
		//TODO: Implement mana overdraw
	}
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, ManaGainBoost, COND_None, REPNOTIFY_Always);
}

void UCharacterAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UCharacterAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UCharacterAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, XP, OldXP);
}

void UCharacterAttributeSet::OnRep_ManaGainBoost(const FGameplayAttributeData& OldManaGainBoost)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, ManaGainBoost, OldManaGainBoost);
}
