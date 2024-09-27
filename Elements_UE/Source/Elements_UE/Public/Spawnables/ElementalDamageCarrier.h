// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spawnables/ElementalActor.h"

#include "GameplayEffect.h"
#include "ElementalDamageCarrier.generated.h"

/**
 * Actor that would be created from an ability and recieve a damage effect to apply to other actors
 */
UCLASS()
class ELEMENTS_UE_API AElementalDamageCarrier : public AElementalActor
{
	GENERATED_BODY()
public:
	AElementalDamageCarrier();

	//bool CanApplyDamageMultipleTimes() const { return bCanApplyDamageMultipleTimes; }

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;


	virtual bool CanTargetActor(AActor* TargetActor);

	UAbilitySystemComponent* GetAbilitySystemComponentFrom(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual bool TryApplyDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual bool TryApplyDamageFromHitResult(AActor* TargetActor, const FHitResult& InHitResult);

	bool DoesCarrierMove() const { return bDoesCarrierMove; }

	//NOT used yet, but could be useful for optimization
	virtual void ResetForPooling();

	virtual FHitResult GetHitResultFromActor(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent, Category = "Elemental Damage Carrier")
	void OnDamageEffectApplied(AActor* Target, UAbilitySystemComponent* TargetASC, FHitResult HitResult);

	void OnDamageEffectApplied_Implementation(AActor* Target, UAbilitySystemComponent* TargetASC, FHitResult HitResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Elemental Damage Carrier")
	bool bDoesCarrierMove = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Elemental Damage Carrier")
	bool bAllowsMultipleHits = false;

	UPROPERTY(EditDefaultsOnly, Category = "Elemental Damage Carrier")
	bool bAllowFriendlyFireForPlayers = false;


	UPROPERTY(EditDefaultsOnly, Category = "Elemental Damage Carrier")
	bool bAllowFriendlyFireForEnemies = true;



private:
	TArray<AActor*> ActorsAlreadyHit;
};
