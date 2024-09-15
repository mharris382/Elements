// Fill out your copyright notice in the Description page of Project Settings.


#include "Mana/ManaPickup.h"
#include "Characters/CharacterBase.h"

// Sets default values
AManaPickup::AManaPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManaPickup::BeginPlay()
{
	Super::BeginPlay();
	
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
	if (characterElement.IsValid() && characterElement.MatchesTagExact(ElementTag))
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
}
