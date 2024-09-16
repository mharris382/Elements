// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ElementalMovementComponent.h"

#include "Characters/CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"

UElementalMovementComponent::UElementalMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	ADSSpeedMultiplier = 0.5f;
}

float UElementalMovementComponent::GetMaxSpeed() const
{
	ACharacterBase* Owner = Cast<ACharacterBase>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"))))
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	if (RequestToStartADS)
	{
		return Owner->GetMoveSpeed() * ADSSpeedMultiplier;
	}

	return Owner->GetMoveSpeed();
}

void UElementalMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UElementalMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UElementalMovementComponent* MutableThis = const_cast<UElementalMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGDNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UElementalMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UElementalMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UElementalMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UElementalMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

void UElementalMovementComponent::FGDSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UElementalMovementComponent::FGDSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UElementalMovementComponent::FGDSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGDSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FGDSavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UElementalMovementComponent::FGDSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UElementalMovementComponent* CharacterMovement = Cast<UElementalMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UElementalMovementComponent::FGDSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UElementalMovementComponent* CharacterMovement = Cast<UElementalMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UElementalMovementComponent::FGDNetworkPredictionData_Client::FGDNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UElementalMovementComponent::FGDNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGDSavedMove());
}
