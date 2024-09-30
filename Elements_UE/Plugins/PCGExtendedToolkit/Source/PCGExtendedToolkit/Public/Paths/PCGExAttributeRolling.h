﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGExPathProcessor.h"
#include "PCGExPaths.h"
#include "PCGExPointsProcessor.h"
#include "Data/Blending/PCGExDataBlending.h"
#include "PCGExAttributeRolling.generated.h"

namespace PCGExDataBlending
{
	class FMetadataBlender;
}

UENUM(BlueprintType, meta=(DisplayName="[PCGEx] Path Shrink Distance Cut Type"))
enum class EPCGExRollingTriggerMode : uint8
{
	None  = 0 UMETA(DisplayName = "None", ToolTip="Ignore triggers"),
	Hold  = 1 UMETA(DisplayName = "Hold", ToolTip="Hold value until next trigger"),
	Reset = 2 UMETA(DisplayName = "Reset", ToolTip="Reset rolling"),
};

/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Path")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExAttributeRollingSettings : public UPCGExPathProcessorSettings
{
	GENERATED_BODY()

public:
	UPCGExAttributeRollingSettings(const FObjectInitializer& ObjectInitializer);
	
	//~Begin UPCGSettings
#if WITH_EDITOR
	PCGEX_NODE_INFOS(AttributeRolling, "Path : Attribute Rolling", "Does a rolling blending of properties & attributes.");
#endif


protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings

	//~Begin UPCGExPointsProcessorSettings
public:
	virtual PCGExData::EInit GetMainOutputInitMode() const override;
	//~End UPCGExPointsProcessorSettings

public:
	/** Blending settings used to smooth attributes.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable, ShowOnlyInnerProperties))
	FPCGExBlendingDetails BlendingSettings = FPCGExBlendingDetails(EPCGExDataBlendingType::Sum, EPCGExDataBlendingType::None);
};

struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExAttributeRollingContext final : public FPCGExPathProcessorContext
{
	friend class FPCGExAttributeRollingElement;

	virtual ~FPCGExAttributeRollingContext() override;
};

class /*PCGEXTENDEDTOOLKIT_API*/ FPCGExAttributeRollingElement final : public FPCGExPathProcessorElement
{
public:
	virtual FPCGContext* Initialize(
		const FPCGDataCollection& InputData,
		TWeakObjectPtr<UPCGComponent> SourceComponent,
		const UPCGNode* Node) override;

protected:
	virtual bool Boot(FPCGExContext* InContext) const override;
	virtual bool ExecuteInternal(FPCGContext* Context) const override;
};

namespace PCGExAttributeRolling
{
	class FProcessor final : public PCGExPointsMT::FPointsProcessor
	{
		FPCGExAttributeRollingContext* LocalTypedContext = nullptr;
		const UPCGExAttributeRollingSettings* LocalSettings = nullptr;

		PCGExPaths::FPathMetrics CurrentMetric;
		PCGExDataBlending::FMetadataBlender* MetadataBlender = nullptr;

		UPCGMetadata* OutMetadata = nullptr;
		TArray<FPCGPoint>* OutPoints = nullptr;

	public:
		explicit FProcessor(PCGExData::FPointIO* InPoints):
			FPointsProcessor(InPoints)
		{
		}

		virtual ~FProcessor() override;

		virtual bool Process(PCGExMT::FTaskManager* AsyncManager) override;
		virtual void PrepareSingleLoopScopeForPoints(const uint32 StartIndex, const int32 Count) override;
		virtual void ProcessSinglePoint(const int32 Index, FPCGPoint& Point, const int32 LoopIdx, const int32 LoopCount) override;
		virtual void CompleteWork() override;
	};
}
