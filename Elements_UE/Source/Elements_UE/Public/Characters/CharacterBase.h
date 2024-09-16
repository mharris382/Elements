// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "ElementSubsystem.h"
#include "Abilities/ElementsGameplayAbility.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EHitReactDirection, Direction);

UCLASS()
class ELEMENTS_UE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Elements|Character")
	FGameplayTag CharacterElementTag;


	UPROPERTY(BlueprintAssignable, Category = "Elements|Character")
	FCharacterDiedDelegate OnCharacterDied;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UCharacterAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual bool IsAlive() const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Property replication */

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	void OnRep_CharacterElementTag();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	virtual FGameplayTag GetCharacterElement();

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

	UFUNCTION(BlueprintCallable, Category = "Elements|Character")
	bool CanMove();

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Elements|Character|Attributes")
	float GetMoveSpeedBaseValue() const;

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

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	/**
* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
* These change the Attribute's Base Value.
*/
	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);

private:
	FGameplayTag NoMovementTag;
};
