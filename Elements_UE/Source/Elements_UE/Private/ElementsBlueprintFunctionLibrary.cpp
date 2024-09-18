// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsBlueprintFunctionLibrary.h"
#include "Interfaces/ElementInterface.h"


FGameplayTag UElementsBlueprintFunctionLibrary::GetElementTag(const UObject* WorldContextObject, FGameplayTagContainer TagContainer)
{
	TObjectPtr<UElementSubsystem> ES;
	if (UElementsBlueprintFunctionLibrary::GetElementSubsystem(WorldContextObject, ES))
	{
		return ES->GetElementTag(TagContainer);
	}
	return FGameplayTag();
}

bool UElementsBlueprintFunctionLibrary::GetElementDataFromTag(const UObject* WorldContextObject, FGameplayTag ElementTag, FElementData& OutElementData)
{
	TObjectPtr<UElementSubsystem> ES;
	if (UElementsBlueprintFunctionLibrary::GetElementSubsystem(WorldContextObject, ES))
	{
		return ES->GetElementDataFromTag(ElementTag, OutElementData);
	}
	return false;
}

bool UElementsBlueprintFunctionLibrary::GetElementDataFromActor(AActor* Actor, FElementData& OutElementData)
{
	IElementInterface* ActorElementInterface; 
	if (!Actor)
	{
		return false;
	}
	ActorElementInterface = Cast<IElementInterface>(Actor);
	if (!ActorElementInterface)
	{
		return false;
	}
	TObjectPtr<UElementSubsystem> ES;
	if (UElementsBlueprintFunctionLibrary::GetElementSubsystem(Actor, ES))
	{
		return ES->GetElementDataFromTag(ActorElementInterface->GetElementTag(), OutElementData);
	}
	return false;
}

TEnumAsByte<EElementRelationship> UElementsBlueprintFunctionLibrary::GetElementRelationship(const UObject* WorldContextObject, FGameplayTag Attacker, FGameplayTag Defender)
{
	TObjectPtr<UElementSubsystem> ES;
	if (UElementsBlueprintFunctionLibrary::GetElementSubsystem(WorldContextObject, ES))
	{
		return ES->GetElementRelationship(Attacker, Defender);
	}
	return TEnumAsByte<EElementRelationship>();
}

TEnumAsByte<EElementRelationship> UElementsBlueprintFunctionLibrary::GetElementRelationshipBetweenActors( AActor* Attacker, AActor* Defender)
{
	if (!Attacker || !Defender) {
		return EElementRelationship::Neutral;
	}
	IElementInterface* AttackerElementInterface = Cast<IElementInterface>(Attacker);
	IElementInterface* DefenderElementInterface = Cast<IElementInterface>(Defender);
	
	if(!AttackerElementInterface || !DefenderElementInterface)
	{
		return EElementRelationship::Neutral;
	}

	TObjectPtr<UElementSubsystem> ES;
	if (UElementsBlueprintFunctionLibrary::GetElementSubsystem(Attacker, ES))
	{
		FGameplayTag AttackerTag = AttackerElementInterface->GetElementTag();
		FGameplayTag DefenderTag = DefenderElementInterface->GetElementTag();
		return UElementsBlueprintFunctionLibrary::GetElementRelationship(Attacker, AttackerTag, DefenderTag);
	}
	return TEnumAsByte<EElementRelationship>();
}

bool UElementsBlueprintFunctionLibrary::GetElementSubsystem(const UObject* WorldContextObject, TObjectPtr<UElementSubsystem>& subsystem)
{
	if(!WorldContextObject)
	{
		return false;
	}
	UWorld* World = WorldContextObject->GetWorld();
	if (World) {
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			UElementSubsystem* ElementSubsystem = GameInstance->GetSubsystem<UElementSubsystem>();
			if (ElementSubsystem)
			{
				subsystem = ElementSubsystem;
				return true;
			}
		}
	}
	return false;
}

