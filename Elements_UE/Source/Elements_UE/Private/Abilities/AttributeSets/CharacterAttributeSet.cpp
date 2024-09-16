// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
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

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, XP, COND_None, REPNOTIFY_Always);
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
