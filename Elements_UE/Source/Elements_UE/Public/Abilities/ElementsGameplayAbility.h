// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ElementsGameplayAbility.generated.h"


UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	Jump			UMETA(DisplayName = "Jump"),
	Dash			UMETA(DisplayName = "Dash"),
	Cast1			UMETA(DisplayName = "QuickCast"),
	Cast2			UMETA(DisplayName = "HeavyCast"),
	Cast3			UMETA(DisplayName = "SpecialCast"),
	Block			UMETA(DisplayName = "Block"),
	Parry			UMETA(DisplayName = "Parry"),
	SwitchElement	UMETA(DisplayName = "SwitchElement")
};


/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UElementsGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UElementsGameplayAbility();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	EAbilityInputID InputID = EAbilityInputID::None;

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
