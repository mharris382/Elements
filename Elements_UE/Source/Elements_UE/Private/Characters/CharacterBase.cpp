// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "ElementsGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/ElementsAbilitySystemComponent.h"
#include "Abilities/AttributeSets/AttributeSetBase.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"
#include "Characters/ElementalMovementComponent.h"
#include "InputActionValue.h"

// Sets default values
ACharacterBase::ACharacterBase(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass < UElementalMovementComponent > (ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
	NoMovementTag = FGameplayTag::RequestGameplayTag(FName("State.NoMovement"));
}

FGameplayTag ACharacterBase::GetElementTag()
{
	return GetCharacterElement();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UCharacterAttributeSet* ACharacterBase::GetAttributeSetBase() const
{
	return AttributeSetBase.Get();
}

bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0;
}

void ACharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterElement(CharacterElementTag);
	
}

void ACharacterBase::SetCharacterElement(FGameplayTag InElementTag)
{
	if (IsLocallyControlled())
	{
		FGameplayTag OldElementTag = CharacterElementTag;
		CharacterElementTag = InElementTag;
		CharacterElementChanged(OldElementTag, InElementTag);
	}
	
	//TODO: ServerSetCharacterElement(ElementTag);
	UWorld* World = GetWorld();
	if (World) {
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			UElementSubsystem* ElementSubsystem = GameInstance->GetSubsystem<UElementSubsystem>();
			if (ElementSubsystem && ElementSubsystem->IsValidElement(InElementTag))
			{
				FGameplayTag OldElementTag = CharacterElementTag;
				CharacterElementTag = InElementTag;
				FElementData ElementData;
				CharacterElementChanged(OldElementTag, InElementTag);
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("ACharacterBase::SetCharacterElement: GameInstance not valid"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ACharacterBase::SetCharacterElement: World not valid"));
	}
	
}

void ACharacterBase::CharacterElementChanged(FGameplayTag OldElementTag, FGameplayTag NewElementTag)
{

	UWorld* World = GetWorld();
	if (World) 
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			UElementSubsystem* ElementSubsystem = GameInstance->GetSubsystem<UElementSubsystem>();
			FElementData ElementData;
			if (ElementSubsystem && ElementSubsystem->GetElementDataFromTag(NewElementTag, ElementData))
			{
				if (AbilitySystemComponent.IsValid() && GetLocalRole() == ROLE_Authority)
				{
					AbilitySystemComponent->SetTagMapCount(OldElementTag, 0);
					AbilitySystemComponent->SetTagMapCount(NewElementTag, 1);
				}
				UpdateCharacterElementVisuals(NewElementTag, ElementData);
			}
		}
	}
}

void ACharacterBase::OnRep_CharacterElementTag(const FGameplayTag& OldCharacterElementTag)
{
	SetCharacterElement(CharacterElementTag);
}

void ACharacterBase::UpdateCharacterElementVisuals_Implementation(FGameplayTag NewElement, FElementData ElementData)
{
}

void ACharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	if (CharacterAbilities.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACharacterBase::AddCharacterAbilities: No default abilities assigned to character: %s"), *GetName());
		return;
	}
	for (TSubclassOf<UElementsGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->InputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ACharacterBase::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		if (GEngine) {
			FString Message1 = FString::Printf(TEXT("No Ability System Component on %s!!!"), *GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,Message1);
		}
		UE_LOG(LogTemp, Error, TEXT("%s() Missing Ability System Component."), *FString(__FUNCTION__), *GetName());
		return;
	}
	if (!DefaultAttributes)
	{
		if (GEngine) {
			FString Message2 = FString::Printf(TEXT("No Default Attributes on %s!!!"), *GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message2);
		}
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ACharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
		AbilitySystemComponent->bStartupEffectsApplied = true;
	}
}

void ACharacterBase::AttackActionStarted()
{
	OnAttackActionStarted.Broadcast();
}

void ACharacterBase::AttackActionCompleted(bool WasSuccessful)
{
	OnAttackActionCompleted.Broadcast(WasSuccessful);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ACharacterBase, CharacterElementTag, COND_None, REPNOTIFY_Always);
	//DOREPLIFETIME(ACharacterBase, CharacterElementTag);
}



void ACharacterBase::Die()
{
	RemoveCharacterAbilities();
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Character Died!!!"));
	}*/
	
	OnCharacterDied.Broadcast(this);

	UWorld* world = GetWorld();
	if (world) {
		AElementsGameMode* GameMode = Cast<AElementsGameMode>(world->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->DropManaFromCharacterDeath(this);
		}
	}

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAllAbilities();
		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);

		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else 
	{
		FinishDying();
	}
}

void ACharacterBase::FinishDying()
{
	Destroy();
}

FGameplayTag ACharacterBase::GetCharacterElement()
{
	//TODO: replace this with dynamic element if element changes (i.e. mage switches element)
	return CharacterElementTag;
}

float ACharacterBase::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ACharacterBase::GetMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMana();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxMana() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxMana();
	}
	return 0.0f;
}

float ACharacterBase::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetCharacterLevel();
	}
	return 1;
}

float ACharacterBase::GetArmor() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetArmor();
	}
	return 0.0f;
}

float ACharacterBase::GetGold() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetGold();
	}
	return 0.0f;
}

float ACharacterBase::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

bool ACharacterBase::CanMove()
{
	if (AbilitySystemComponent.IsValid())
	{
		return !AbilitySystemComponent->HasMatchingGameplayTag(NoMovementTag);
	}
	UE_LOG(LogTemp, Error, TEXT("ACharacterBase::CanMove: AbilitySystemComponent not valid"));
	return true;
}


float ACharacterBase::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}
	return 0.0f;
}


void ACharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void ACharacterBase::SetMana(float Mana)
{
	if (AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMana(Mana);
	}
}
