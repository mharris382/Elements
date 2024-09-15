// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ElementsGameplayAbility_Jump.h"
#include "Characters/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UElementsGameplayAbility_Jump::UElementsGameplayAbility_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	InputID = EAbilityInputID::Jump;
}

void UElementsGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}

		ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
		Character->bPressedJump = true;
		Character->Jump();
	}
}

bool UElementsGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	const ACharacterBase* Character = CastChecked<ACharacterBase>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullChecked);
	return Character && Character->CanJump();
}

void UElementsGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != NULL && ActorInfo->AvatarActor != NULL)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UElementsGameplayAbility_Jump::InputReleased: ActorInfo or AvatarActor is null"));
	}
}

void UElementsGameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UElementsGameplayAbility_Jump::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	ACharacter* Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());
	if (Character) 
	{
		Character->bPressedJump = false;
		Character->StopJumping();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("UElementsGameplayAbility_Jump::CancelAbility: AvatarActor is not a Character"));
	}
	
}
