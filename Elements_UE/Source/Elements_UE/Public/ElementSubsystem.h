// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ElementSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FElementData : public FTableRowBase
{
    GENERATED_BODY()


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
    FName DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
    FGameplayTag ElementTag;

    //color to represent the element, used for UI and to drive visual effects 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
    FLinearColor ElementColorID;

    //comma separated list of elements that this element is strong against
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Element")
    FGameplayTagContainer StrongAgainst;

};

UENUM(BlueprintType)
enum EElementRelationship : uint8
{
    Neutral,
    Strong,
    Weak
};

/**
 * 
 */
UCLASS()
class ELEMENTS_UE_API UElementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
    
public:
    // Initialize and deinitialize the subsystem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void InitializeElementRelationships();

    UFUNCTION(BlueprintCallable, Category = "Element")
    TEnumAsByte<EElementRelationship> GetElementRelationship(FGameplayTag Attacker, FGameplayTag Defender);



    UFUNCTION(BlueprintCallable, Category = "Element")
    void SpawnManaPickup(FVector Location, FGameplayTag ElementTag, float ManaAmount, AActor* Source);

    
    bool GetElementDataFromTag(FGameplayTag ElementTag, FElementData& OutElementData);

private:
    // Store the loaded element data
    UPROPERTY()
    TObjectPtr<UDataTable> ElementDataTable;

    TSubclassOf<class AManaPickup> ManaPickupClass;

    FGameplayTag ElementTagBase;
    TMap<FGameplayTag, FGameplayTagContainer> ElementStrongAgainst;
};
