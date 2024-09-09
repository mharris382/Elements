// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementsSubsystem.h"

void UElementsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    FString DataTablePath = TEXT("/Game/Data/DT_ElementData");
    ElementDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
    if (ElementDataTable)
    {
        UE_LOG(LogTemp, Log, TEXT("Element Data Table Loaded"));
    }
    else {
        UE_LOG(LogTemp, Log, TEXT("Element Data Table Not Loaded"));
    }
    InitializeElementRelationships();
}

void UElementsSubsystem::Deinitialize()
{
    Super::Deinitialize();

    ElementDataTable = nullptr;
}


void UElementsSubsystem::InitializeElementRelationships()
{
    // Iterate through each row in the DataTable
    TArray<FElementData*> AllElements;
    ElementDataTable->GetAllRows(TEXT(""), AllElements);
    
    for (FElementData* Element : AllElements)
    {
        ElementStrongAgainst.Add(Element->ElementTag, Element->StrongAgainst);
        
    }

    // Log for debugging
    UE_LOG(LogTemp, Log, TEXT("Element relationships initialized"));
}

TEnumAsByte<EElementRelationship> UElementsSubsystem::GetElementRelationship(FGameplayTag Attacker, FGameplayTag Defender)
{
    FGameplayTagContainer StrongAgainst = ElementStrongAgainst[Attacker];
    FGameplayTagContainer WeakAgainst = ElementStrongAgainst[Defender];

    if(Attacker.MatchesTag(Defender))
	{
		return EElementRelationship::Weak;
	}

    if(StrongAgainst.HasTag(Defender))
	{
        return EElementRelationship::Strong;
	}
	else if(WeakAgainst.HasTag(Defender))
	{
		return EElementRelationship::Weak;
	}
	else
	{
		return EElementRelationship::Neutral;
	}
}
