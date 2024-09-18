// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ElementSubsystem.h"
#include "ElementsBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UElementsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Element")
	static FGameplayTag GetElementTag(const UObject* WorldContextObject, FGameplayTagContainer TagContainer);
	
	UFUNCTION(BlueprintCallable, Category = "Element")
	static bool GetElementDataFromTag(const UObject* WorldContextObject, FGameplayTag ElementTag, FElementData& OutElementData);
	
	UFUNCTION(BlueprintCallable, Category = "Element")
	static bool GetElementDataFromActor(AActor* Actor, FElementData& OutElementData);

	UFUNCTION(BlueprintCallable, Category = "Element")
	static TEnumAsByte<EElementRelationship> GetElementRelationship(const UObject* WorldContextObject, FGameplayTag Attacker, FGameplayTag Defender);
	
	UFUNCTION(BlueprintCallable, Category = "Element")
	static TEnumAsByte<EElementRelationship> GetElementRelationshipBetweenActors(AActor* Attacker, AActor* Defender);
	

	static bool GetElementSubsystem(const UObject* WorldContextObject, TObjectPtr<UElementSubsystem>& subsystem);
};
