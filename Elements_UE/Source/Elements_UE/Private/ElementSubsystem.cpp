// Fill out your copyright notice in the Description page of Project Settings.


#include "ElementSubsystem.h"

void UElementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
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
	ElementTagBase = FGameplayTag::RequestGameplayTag(FName("Elements"));
	if (ElementTagBase.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Element Tag Base Loaded"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Element Tag Base Not Loaded"));
	}
	InitializeElementRelationships();
}

void UElementSubsystem::Deinitialize()
{
	Super::Deinitialize();

	ElementDataTable = nullptr;
}

void UElementSubsystem::InitializeElementRelationships()
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

TEnumAsByte<EElementRelationship> UElementSubsystem::GetElementRelationship(FGameplayTag Attacker, FGameplayTag Defender)
{
	FGameplayTagContainer StrongAgainst = ElementStrongAgainst[Attacker];
	FGameplayTagContainer WeakAgainst = ElementStrongAgainst[Defender];

	if (Attacker.MatchesTag(Defender))
	{
		return EElementRelationship::Weak;
	}

	if (StrongAgainst.HasTag(Defender))
	{
		return EElementRelationship::Strong;
	}
	else if (WeakAgainst.HasTag(Defender))
	{
		return EElementRelationship::Weak;
	}
	else
	{
		return EElementRelationship::Neutral;
	}
}

bool UElementSubsystem::GetElementDataFromTag(FGameplayTag ElementTag, FElementData& OutElementData)
{
	if (ElementTag.IsValid() && ElementTag.MatchesTag(ElementTagBase))
	{
		// Get the full tag name, e.g., "Element.Fire"
		FName TagFullName = ElementTag.GetTagName();

		// Convert to string for easy manipulation
		FString TagFullNameStr = TagFullName.ToString();

		// Find the last '.' in the tag name and extract the element part (e.g., "Fire")
		FString ElementName;
		if (TagFullNameStr.Split(TEXT("."), nullptr, &ElementName))
		{
			// Convert the element part to an FName for lookup in the data table
			FName RowName = FName(*ElementName);

			// Attempt to find the row in the data table
			FElementData* ElementData = ElementDataTable->FindRow<FElementData>(RowName, TEXT(""));

			// If data is found, assign it to the output and return true
			if (ElementData)
			{
				OutElementData = *ElementData;
				return true;  // Element data found
			}
		}
	}
	return false;
}

