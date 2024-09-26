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

bool AElementalDamageCarrier::CanTargetActor(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return false;
	}

	if (TargetActor == GetInstigator())
	{
		return false;
	}
	if (!bAllowsMultipleHits && ActorsAlreadyHit.Contains(TargetActor))
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

	ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor);
	if (TargetCharacter) 
	{
		if (!TargetCharacter->IsAlive())
		{
			return false;
		}
		UAbilitySystemComponent* OutASC = GetAbilitySystemComponentFrom(TargetActor);
		if (!OutASC)
		{
			return false;
		}
		return true;
	}
	
	//if(TargetActor->Implements<UAbilitySystemInterface>())
	//{
	//	const UAbilitySystemComponent* AbilitySystemComponent = Cast<IAbilitySystemInterface>(TargetActor)->GetAbilitySystemComponent();
	//	//must have ability system component
	//	if (!AbilitySystemComponent)
	//	{
	//		return false;
	//	}
	//	const UAttributeSetBase* AttributeSet = AbilitySystemComponent->GetSet<UAttributeSetBase>();
	//	//must have attribute set with health
	//	if (!AttributeSet) 
	//	{
	//		return false;
	//	}
	//	//cannot apply if dead
	//	if(AttributeSet->GetHealth() <= 0.0f)
	//	{
	//		return false;
	//	}
	//
	//	return true;
	//}
	return false;
}

UAbilitySystemComponent* AElementalDamageCarrier::GetAbilitySystemComponentFrom(AActor* TargetActor)
{
	ACharacterBase* TargetCharacter = Cast<ACharacterBase>(TargetActor);
	if (TargetCharacter) {
		return TargetCharacter->GetAbilitySystemComponent();
	}
	return nullptr;
}

bool AElementalDamageCarrier::TryApplyDamage(AActor* TargetActor)
{
	if (!CanTargetActor(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AElementalDamageCarrier::TryApplyDamage: Cannot target actor %s"), *GetNameSafe(TargetActor));
		return false;
	}
	FHitResult HitResult = GetHitResultFromActor(TargetActor);
	return TryApplyDamageFromHitResult(TargetActor, HitResult);
}

bool AElementalDamageCarrier::TryApplyDamageFromHitResult(AActor* TargetActor, const FHitResult& InHitResult)
{
	if (!CanTargetActor(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AElementalDamageCarrier::TryApplyDamage: Cannot target actor %s"), *GetNameSafe(TargetActor));
		return false;
	}

	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponentFrom(TargetActor);
	if (GetLocalRole() == ROLE_Authority)
	{
		FActiveGameplayEffectHandle ageh = TargetASC->BP_ApplyGameplayEffectSpecToSelf(DamageEffectSpecHandle);
	}
	ActorsAlreadyHit.Add(TargetActor);
	OnDamageEffectApplied(TargetActor, TargetASC, InHitResult);
	return true;
}

void AElementalDamageCarrier::ResetForPooling()
{
	ActorsAlreadyHit.Empty();
}

FHitResult AElementalDamageCarrier::GetHitResultFromActor(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return FHitResult();
	}
	UPrimitiveComponent* Primative = TargetActor->FindComponentByClass<UPrimitiveComponent>();
	if (Primative)
	{
		return FHitResult(TargetActor, Primative, TargetActor->GetActorLocation(), FVector(0,0,1));
	}
	return FHitResult();
}

void AElementalDamageCarrier::OnDamageEffectApplied_Implementation(AActor* Target, UAbilitySystemComponent* TargetASC, FHitResult HitResult)
{
}
