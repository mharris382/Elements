// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawnables/ElementalDamageCarrier.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Characters/Mage.h"
#include "Abilities/AttributeSets/AttributeSetBase.h"
#include "AbilitySystemInterface.h"


AElementalDamageCarrier::AElementalDamageCarrier()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
	SetReplicateMovement(bDoesCarrierMove);
}

bool AElementalDamageCarrier::CanTargetActor(AActor* TargetActor) const
{
	if (!TargetActor)
		return false;

	if (TargetActor == GetInstigator())
	{
		return false;
	}

	//if target and source are both players, do not allow damage
	if (!bAllowFriendlyFireForPlayers && GetInstigator()) {
		AMage* TargetMage = Cast<AMage>(TargetActor);
		AMage* SourceMage = Cast<AMage>(GetInstigator());
		if (TargetMage && SourceMage)
		{
			return false;
		}
	}
	
	if(TargetActor->Implements<UAbilitySystemInterface>())
	{
		const UAbilitySystemComponent* AbilitySystemComponent = Cast<IAbilitySystemInterface>(TargetActor)->GetAbilitySystemComponent();
		//must have ability system component
		if (!AbilitySystemComponent)
		{
			return false;
		}
		const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
		//must have attribute set with health
		if (!AttributeSet) 
		{
			return false;
		}
		//cannot apply if dead
		if(AttributeSet->GetHealth() <= 0.0f)
		{
			return false;
		}

		return true;
	}
	return false;
}

bool AElementalDamageCarrier::TryApplyDamage(AActor* TargetActor)
{
	return false;
}

bool AElementalDamageCarrier::TryApplyDamageFromHitResult(AActor* TargetActor, const FHitResult& InHitResult)
{
	return false;
}

void AElementalDamageCarrier::ResetForPooling()
{
	ActorsAlreadyHit.Empty();
}
