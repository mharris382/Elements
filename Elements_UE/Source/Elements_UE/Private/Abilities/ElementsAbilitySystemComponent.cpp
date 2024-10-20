// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ElementsAbilitySystemComponent.h"

void UElementsAbilitySystemComponent::ReceiveDamage(UElementsAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage, EElementRelationship DamageRelationship)
{
	//TODO: Broadcast this event to the UI
	OnReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage, DamageRelationship);
}
