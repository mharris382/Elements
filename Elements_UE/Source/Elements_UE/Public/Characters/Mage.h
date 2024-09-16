// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Abilities/ElementsGameplayAbility.h"
#include "Mage.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UENUM(BlueprintType)
enum class EInputType : uint8
{
	Trigger UMETA(DisplayName = "Trigger"),
	Hold UMETA(DisplayName = "Hold")
};




USTRUCT(BlueprintType)
struct FInputAbilityMapping
{
	GENERATED_BODY()

public:
	// The input action that will trigger the ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* InputAction;

	// The gameplay tag associated with the ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AbilityTags;

	// Specify if the input is a trigger or hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputType InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityInputID InputID;

	// Default constructor
	FInputAbilityMapping()
		: InputAction(nullptr)
	{}
};

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API AMage : public ACharacterBase
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;


	virtual void FinishDying() override;

public:
	AMage(const class FObjectInitializer& ObjectInitializer);

	// Array of input-action mappings to ability tags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FInputAbilityMapping> InputAbilityMappings;

protected:

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


	void BeginJump();

	void EndJump();

	virtual void HandleAbilityInputPressed(EAbilityInputID InputID);
	virtual void HandleAbilityInputReleased(EAbilityInputID InputID);

private: 
	TMap<FGameplayTag, FInputAbilityMapping> AbilityTagToInputActionMap;
};
