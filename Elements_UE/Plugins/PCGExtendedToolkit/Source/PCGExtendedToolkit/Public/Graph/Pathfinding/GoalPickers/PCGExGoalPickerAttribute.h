﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PCGExGoalPicker.h"
#include "Data/PCGExAttributeHelpers.h"
#include "PCGExGoalPickerAttribute.generated.h"

struct FPCGPoint;
class UPCGPointData;

UENUM(BlueprintType, meta=(DisplayName="[PCGEx] Goal Pick Attribute - Amount"))
enum class EPCGExGoalPickAttributeAmount : uint8
{
	Single = 0 UMETA(DisplayName = "Single Attribute", Tooltip="Single attribute"),
	List   = 1 UMETA(DisplayName = "Multiple Attributes", Tooltip="Multiple attributes"),
};

/**
 * 
 */
UCLASS(MinimalAPI, DisplayName = "Index Attribute")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExGoalPickerAttribute : public UPCGExGoalPicker
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GoalPicker)
	EPCGExGoalPickAttributeAmount GoalCount = EPCGExGoalPickAttributeAmount::Single;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GoalPicker, meta=(EditCondition="GoalCount==EPCGExGoalPickAttributeAmount::Single", EditConditionHides))
	FPCGAttributePropertyInputSelector SingleSelector;
	PCGExData::TCache<double>* SingleGetter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = GoalPicker, meta=(EditCondition="GoalCount==EPCGExGoalPickAttributeAmount::List", EditConditionHides, TitleProperty="{TitlePropertyName}"))
	TArray<FPCGAttributePropertyInputSelector> AttributeSelectors;
	TArray<PCGExData::TCache<double>*> AttributeGetters;

	virtual void CopySettingsFrom(const UPCGExOperation* Other) override;

	virtual void PrepareForData(PCGExData::FFacade* InSeedsDataFacade, PCGExData::FFacade* InGoalsDataFacade) override;
	virtual int32 GetGoalIndex(const PCGExData::FPointRef& Seed) const override;
	virtual void GetGoalIndices(const PCGExData::FPointRef& Seed, TArray<int32>& OutIndices) const override;
	virtual bool OutputMultipleGoals() const override;
};
