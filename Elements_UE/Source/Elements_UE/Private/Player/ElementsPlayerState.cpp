// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ElementsPlayerState.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "Abilities/AttributeSets/AttributeSetBase.h"
#include "Characters/Mage.h"

AElementsPlayerState::AElementsPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UElementsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another ElementsPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 50.0f;

	// Cache tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AElementsPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSet* AElementsPlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}

bool AElementsPlayerState::IsAlive() const
{
	return GetHealth() > 0;
}

float AElementsPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AElementsPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AElementsPlayerState::GetHealthRegen() const
{
	return AttributeSet->GetHealthRegenRate();
}

float AElementsPlayerState::GetMana() const
{
	return AttributeSet->GetMana();
}

float AElementsPlayerState::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AElementsPlayerState::GetManaRegen() const
{
	return AttributeSet->GetManaRegenRate();
}

void AElementsPlayerState::BeginPlay()
{
}

void AElementsPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement health GUI

	AMage* Mage = Cast<AMage>(GetPawn());
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (Mage) 
		{
			Mage->Die();
		}
	}
	
}

void AElementsPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement health GUI
}

void AElementsPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement health GUI
}

void AElementsPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement mana GUI
}

void AElementsPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement mana GUI
}

void AElementsPlayerState::ManaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	//TODO: implement mana GUI
}

void AElementsPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}
