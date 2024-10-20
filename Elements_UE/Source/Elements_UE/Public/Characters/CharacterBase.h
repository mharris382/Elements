// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ElementSubsystem.h"
#include "Interfaces/ElementInterface.h"
#include "Abilities/ElementsGameplayAbility.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElementChangedDelegate, FGameplayTag, ElementTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackActionStartedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackActionCompleted, bool, bActionSuccess);

UCLASS()
class ELEMENTS_UE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public IElementInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);


#pragma region CharacterElementType


	FGameplayTag GetElementTag();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Elements|Character", ReplicatedUsing = OnRep_CharacterElementTag, Meta = (Categories = "Characters.Archetype"))
	FGameplayTag CharacterArchetypeTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Character", ReplicatedUsing = OnRep_CharacterElementTag, Meta = (Categories = "Elements"))
	FGameplayTag CharacterElementTag;

	UFUNCTION()
	void OnRep_CharacterElementTag(const FGameplayTag& OldCharacterElementTag);

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual FGameplayTag GetCharacterElement();


	UPROPERTY(BlueprintAssignable, Category = "Elements|Character")
	FElementChangedDelegate OnElementChanged;

#pragma endregion



	//--------------------------------------------------------------------------------------------


	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UCharacterAttributeSet* GetAttributeSetBase() const;



	

	UPROPERTY(BlueprintAssignable, Category = "Elements|Character")
	FAttackActionCompleted OnAttackActionCompleted;


	UPROPERTY(BlueprintAssignable, Category = "Elements|Character")
	FAttackActionStartedDelegate OnAttackActionStarted;

	///used by anim notify to tell ability system to begin the attack action (this could be used for spawning projectile, or activating a melee attack hitbox)
	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	void AttackActionStarted();


	///used by anim notify to tell ability system to complete the attack action
	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	void AttackActionCompleted(bool WasSuccessful);


	//--------------------------------------------------------------------------------------------

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Property replication */

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//--------------------------------------------------------------------------------------------

#pragma region Life And Death


	UPROPERTY(BlueprintAssignable, Category = "Elements|Character")
	FCharacterDiedDelegate OnCharacterDied;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual void FinishDying();


	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual bool IsAlive() const;


	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();


#pragma endregion

#pragma region Attribute Getters

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetArmor() const;


	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetGold() const;


	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMoveSpeed() const;



	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMoveSpeedBaseValue() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	bool CanMove();


#pragma endregion


	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	TWeakObjectPtr<UElementsAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCharacterAttributeSet> AttributeSetBase;

	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;



	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Character")
	FText CharacterName;

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elements|Animation")
	UAnimMontage* DeathMontage;


#pragma region GAS Initialization


	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Abilities")
	TArray<TSubclassOf<UElementsGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();


	virtual void AddStartupEffects();

#pragma endregion

#pragma region CharacterElementType Functions

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	void SetCharacterElement(FGameplayTag ElementTag);

	//UFUNCTION(BlueprintNativeEvent)
	//bool CanSetCharacterElement(FGameplayTag ElementTag);
	//
	//bool CanSetCharacterElement_Implementation(FGameplayTag ElementTag);

	void CharacterElementChanged(FGameplayTag OldElementTag, FGameplayTag NewElementTag);


	UFUNCTION(BlueprintNativeEvent)
	void UpdateCharacterElementVisuals(FGameplayTag NewElement, FElementData ElementData);

	void UpdateCharacterElementVisuals_Implementation(FGameplayTag NewElement, FElementData ElementData);


#pragma endregion


	/**
* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
* These change the Attribute's Base Value.
*/
	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);

private:
	FGameplayTag NoMovementTag;
};


