// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ElementsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AElementsPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AElementsPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UCharacterAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState")
	bool IsAlive() const;


	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetHealthRegen() const;


	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetManaRegen() const;


	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|ElementsPlayerState|Attributes")
	float GetXP() const;

protected:

	UPROPERTY()
	class UElementsAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UCharacterAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle ManaRegenRateChangedDelegateHandle;

	virtual void BeginPlay() override;


	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void ManaRegenRateChanged(const FOnAttributeChangeData& Data);
	
	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
