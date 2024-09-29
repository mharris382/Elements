// Fill out your copyright notice in the Description page of Project Settings.


#include "Mana/ManaPickup.h"
#include "Net/UnrealNetwork.h"
#include "Characters/CharacterBase.h"
#include "Abilities/AttributeSets/CharacterAttributeSet.h"

// Sets default values
AManaPickup::AManaPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
	bReplicates = true;
}

void AManaPickup::OnRep_ElementTag(const FGameplayTag& OldElementTag)
{
	FElementData ElementData;
	UElementSubsystem* ElementSubsystem = GetGameInstance()->GetSubsystem<UElementSubsystem>();
	if (ElementSubsystem)
	{
		// Now you can use ElementSubsystem
		ElementSubsystem->GetElementDataFromTag(ElementTag, ElementData);
		UpdateElementVisuals(ElementData);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Element Subsystem Not Found"));
	}
}

// Called when the game starts or when spawned
void AManaPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AManaPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AManaPickup, ElementTag);
	DOREPLIFETIME_CONDITION_NOTIFY(AManaPickup, ElementTag, COND_None, REPNOTIFY_Always);
}

// Called every frame
void AManaPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManaPickup::SetSourceAndTarget_Implementation(AActor* NewSourceActor, FVector TargetLocation)
{
}

bool AManaPickup::CanBePickedUpBy(ACharacterBase* Character) const
{
	if (!Character) {
		return false;
	}
	FGameplayTag characterElement = Character->GetCharacterElement();
	UCharacterAttributeSet* Attributes = Character->GetAttributeSetBase();
	if (!Attributes)
	{
		UE_LOG(LogTemp, Warning, TEXT("AManaPickup::CanBePickedUpBy: Character %s has no Attributes"), *Character->GetName());
		return false;
	}
	//must be of the same element type and not at max mana
	if (characterElement.IsValid() && characterElement.MatchesTagExact(ElementTag) && Attributes->GetMana() < Attributes->GetMaxMana())
	{
		return true;
	}
	return false;
}

void AManaPickup::SetManaAmount(float NewManaAmount)
{
	ManaAmount = NewManaAmount;
	//Set Element Type to update visuals based on the new mana amount
	SetElementType(ElementTag);
}

void AManaPickup::SetElementType(FGameplayTag NewElementTag)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	else
	{
		OnRep_ElementTag(ElementTag);
	}
	FElementData ElementData;
	UElementSubsystem* ElementSubsystem = GetGameInstance()->GetSubsystem<UElementSubsystem>();
	if (ElementSubsystem)
	{
		// Now you can use ElementSubsystem
		ElementSubsystem->GetElementDataFromTag(NewElementTag, ElementData);
		UpdateElementVisuals(ElementData);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Element Subsystem Not Found"));
	}
	ElementTag = NewElementTag;
}
