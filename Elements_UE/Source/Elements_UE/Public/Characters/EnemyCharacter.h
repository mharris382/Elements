// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Characters/CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()


public:

	AEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	UElementsAbilitySystemComponent* HardRefAbilitySystemComponent;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	UCharacterAttributeSet* HardRefAttributeSetBase;


	virtual void BeginPlay() override;


	FDelegateHandle HealthChangedDelegateHandle;


	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
