﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "Data/Blending/PCGExDataBlending.h"
#include "Graph/PCGExClusterMT.h"
#include "Graph/PCGExEdgesProcessor.h"
#include "Sampling/PCGExSampling.h"
#include "PCGExWriteEdgeProperties.generated.h"

#define PCGEX_FOREACH_FIELD_EDGEEXTRAS(MACRO) \
MACRO(EdgeLength, double) \
MACRO(EdgeDirection, FVector) \
MACRO(Heuristics, double)

namespace PCGExDataBlending
{
	class FMetadataBlender;
	struct FPropertiesBlender;
}

namespace PCGExWriteEdgeProperties
{
	class FProcessorBatch;
}

UENUM(BlueprintType, meta=(DisplayName="[PCGEx] Heuristics Write Mode"))
enum class EPCGExHeuristicsWriteMode : uint8
{
	EndpointsOrder = 0 UMETA(DisplayName = "Endpoints Order", ToolTip="Use endpoint order heuristics."),
	Smallest       = 1 UMETA(DisplayName = "Smallest Score", ToolTip="Compute heuristics both ways a keep smallest score"),
	Highest        = 2 UMETA(DisplayName = "Highest Score", ToolTip="Compute heuristics both ways a keep highest score."),
};

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Edges")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExWriteEdgePropertiesSettings : public UPCGExEdgesProcessorSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings
#if WITH_EDITOR
	PCGEX_NODE_INFOS(WriteEdgeProperties, "Cluster : Edge Properties", "Extract & write extra edge informations to the point representing the edge.");
	virtual FLinearColor GetNodeTitleColor() const override { return GetDefault<UPCGExGlobalSettings>()->NodeColorSamplerNeighbor; }
#endif

	virtual PCGExData::EInit GetMainOutputInitMode() const override;
	virtual PCGExData::EInit GetEdgeOutputInitMode() const override;

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings

public:
	/** Defines the direction in which points will be ordered to form the final paths. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable))
	FPCGExEdgeDirectionSettings DirectionSettings;

	/** Output Edge Length. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteEdgeLength = false;

	/** Name of the 'boolean' attribute to write sampling success to.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="EdgeLength", PCG_Overridable, EditCondition="bWriteEdgeLength"))
	FName EdgeLengthAttributeName = FName("EdgeLength");

	/** Output Edge Direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteEdgeDirection = false;

	/** Name of the 'boolean' attribute to write sampling success to.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="EdgeDirection", PCG_Overridable, EditCondition="bWriteEdgeDirection"))
	FName EdgeDirectionAttributeName = FName("EdgeDirection");

	/** Edges will inherit point attributes*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta = (PCG_Overridable))
	bool bEndpointsBlending = false;

	/** Balance between start/end point ( When enabled, this value will be overriden by EdgePositionLerp, and Solidification, in that order. )*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, EditCondition="bEndpointsBlending && !bWriteEdgePosition && SolidificationAxis == EPCGExMinimalAxis::None", ClampMin=0, ClampMax=1))
	double EndpointsWeights = 0.5;

	/** Defines how fused point properties and attributes are merged together. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(EditCondition="bEndpointsBlending"))
	FPCGExBlendingDetails BlendingSettings = FPCGExBlendingDetails(EPCGExDataBlendingType::Average);

	/** Output Edge Heuristics. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteHeuristics = false;

	/** Name of the 'double' attribute to write heuristics to.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="Heuristics", PCG_Overridable, EditCondition="bWriteHeuristics"))
	FName HeuristicsAttributeName = FName("Heuristics");

	/** Heuristic write mode. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, DisplayName=" └─ Heuristics Mode", EditCondition="bWriteHeuristics", EditConditionHides, HideEditConditionToggle))
	EPCGExHeuristicsWriteMode HeuristicsMode = EPCGExHeuristicsWriteMode::EndpointsOrder;


	/** Update Edge position as a lerp between endpoints (according to the direction method selected above) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta=(PCG_Overridable))
	bool bWriteEdgePosition = false;

	/** Position position lerp between start & end points*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta=(PCG_Overridable, EditCondition="bWriteEdgePosition && SolidificationAxis == EPCGExMinimalAxis::None", ClampMin=0, ClampMax=1))
	double EdgePositionLerp = 0.5;

	/** Align the edge point to the edge direction over the selected axis. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta = (PCG_Overridable))
	EPCGExMinimalAxis SolidificationAxis = EPCGExMinimalAxis::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta=(PCG_Overridable, EditCondition="SolidificationAxis != EPCGExMinimalAxis::None"))
	EPCGExFetchType SolidificationLerpOperand = EPCGExFetchType::Constant;

	/** Solidification Lerp constant.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta=(PCG_Overridable, EditCondition="SolidificationLerpOperand == EPCGExFetchType::Constant && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	double SolidificationLerpConstant = 0.5;

	/** Solidification Lerp attribute (read from Edge).*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification", meta=(PCG_Overridable, EditCondition="SolidificationLerpOperand == EPCGExFetchType::Attribute && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	FPCGAttributePropertyInputSelector SolidificationLerpAttribute;

	// Edge radiuses

	/** Whether or not to write the edge extents over the local X axis.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="SolidificationAxis != EPCGExMinimalAxis::X && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	bool bWriteRadiusX = false;

	/** Type of Radius X value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusX && SolidificationAxis != EPCGExMinimalAxis::X && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	EPCGExFetchType RadiusXType = EPCGExFetchType::Constant;

	/** Source from which to fetch the Radius X value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusX && SolidificationAxis != EPCGExMinimalAxis::X && SolidificationAxis != EPCGExMinimalAxis::None && RadiusXType==EPCGExFetchType::Attribute", EditConditionHides))
	EPCGExGraphValueSource RadiusXSource = EPCGExGraphValueSource::Vtx;

	/** Attribute read on edge endpoints */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusX && SolidificationAxis != EPCGExMinimalAxis::X && SolidificationAxis != EPCGExMinimalAxis::None && RadiusXType==EPCGExFetchType::Attribute", EditConditionHides))
	FPCGAttributePropertyInputSelector RadiusXSourceAttribute;

	/** Radius X Constant */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusX && SolidificationAxis != EPCGExMinimalAxis::X && SolidificationAxis != EPCGExMinimalAxis::None && RadiusXType==EPCGExFetchType::Constant", EditConditionHides))
	double RadiusXConstant = 1;


	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="SolidificationAxis != EPCGExMinimalAxis::Y && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	bool bWriteRadiusY = false;

	/** Type of Radius Y value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusY && SolidificationAxis != EPCGExMinimalAxis::Y && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	EPCGExFetchType RadiusYType = EPCGExFetchType::Constant;

	/** Source from which to fetch the Radius Y value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusY && SolidificationAxis != EPCGExMinimalAxis::Y && SolidificationAxis != EPCGExMinimalAxis::None && RadiusZType==EPCGExFetchType::Attribute", EditConditionHides))
	EPCGExGraphValueSource RadiusYSource = EPCGExGraphValueSource::Vtx;

	/** Attribute read on edge endpoints */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusY && SolidificationAxis != EPCGExMinimalAxis::Y && SolidificationAxis != EPCGExMinimalAxis::None && RadiusZType==EPCGExFetchType::Attribute", EditConditionHides))
	FPCGAttributePropertyInputSelector RadiusYSourceAttribute;

	/** Radius Y Constant */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusY && SolidificationAxis != EPCGExMinimalAxis::Y && SolidificationAxis != EPCGExMinimalAxis::None && RadiusYType==EPCGExFetchType::Constant", EditConditionHides))
	double RadiusYConstant = 1;


	/**  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="SolidificationAxis != EPCGExMinimalAxis::Z && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	bool bWriteRadiusZ = false;

	/** Type of Radius Z value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusZ && SolidificationAxis != EPCGExMinimalAxis::Z && SolidificationAxis != EPCGExMinimalAxis::None", EditConditionHides))
	EPCGExFetchType RadiusZType = EPCGExFetchType::Constant;

	/** Source from which to fetch the Radius Z value */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta=(PCG_Overridable, EditCondition="bWriteRadiusZ && SolidificationAxis != EPCGExMinimalAxis::Z && SolidificationAxis != EPCGExMinimalAxis::None && RadiusZType==EPCGExFetchType::Attribute", EditConditionHides))
	EPCGExGraphValueSource RadiusZSource = EPCGExGraphValueSource::Vtx;

	/** Attribute read on edge endpoints */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusZ && SolidificationAxis != EPCGExMinimalAxis::Z && SolidificationAxis != EPCGExMinimalAxis::None && RadiusZType==EPCGExFetchType::Attribute", EditConditionHides))
	FPCGAttributePropertyInputSelector RadiusZSourceAttribute;

	/** Radius Z Constant */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Solidification|Radiuses", meta = (PCG_Overridable, EditCondition="bWriteRadiusZ && SolidificationAxis != EPCGExMinimalAxis::Z && SolidificationAxis != EPCGExMinimalAxis::None && RadiusZType==EPCGExFetchType::Constant", EditConditionHides))
	double RadiusZConstant = 1;

private:
	friend class FPCGExWriteEdgePropertiesElement;
};

struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExWriteEdgePropertiesContext final : public FPCGExEdgesProcessorContext
{
	friend class FPCGExWriteEdgePropertiesElement;

	virtual ~FPCGExWriteEdgePropertiesContext() override;

	PCGEX_FOREACH_FIELD_EDGEEXTRAS(PCGEX_OUTPUT_DECL_TOGGLE)
};

class /*PCGEXTENDEDTOOLKIT_API*/ FPCGExWriteEdgePropertiesElement final : public FPCGExEdgesProcessorElement
{
public:
	virtual FPCGContext* Initialize(
		const FPCGDataCollection& InputData,
		TWeakObjectPtr<UPCGComponent> SourceComponent,
		const UPCGNode* Node) override;

protected:
	virtual bool Boot(FPCGExContext* InContext) const override;
	virtual bool ExecuteInternal(FPCGContext* InContext) const override;
};

namespace PCGExWriteEdgeProperties
{
	class FProcessor final : public PCGExClusterMT::FClusterProcessor
	{
		const UPCGExWriteEdgePropertiesSettings* LocalSettings = nullptr;

		FPCGExEdgeDirectionSettings DirectionSettings;

		double StartWeight = 0;
		double EndWeight = 1;

		PCGExDataBlending::FMetadataBlender* MetadataBlender = nullptr;

		PCGExData::TCache<double>* SolidificationLerpGetter = nullptr;

		PCGEX_FOREACH_FIELD_EDGEEXTRAS(PCGEX_OUTPUT_DECL)

		bool bSolidify = false;

#define PCGEX_LOCAL_EDGE_GETTER_DECL(_AXIS) PCGExData::TCache<double>* SolidificationRad##_AXIS = nullptr; bool bOwnSolidificationRad##_AXIS = true; double Rad##_AXIS##Constant = 1;
		PCGEX_FOREACH_XYZ(PCGEX_LOCAL_EDGE_GETTER_DECL)
#undef PCGEX_LOCAL_EDGE_GETTER_DECL

	public:
		FProcessor(PCGExData::FPointIO* InVtx, PCGExData::FPointIO* InEdges):
			FClusterProcessor(InVtx, InEdges)
		{
		}

		virtual ~FProcessor() override;

		virtual bool Process(PCGExMT::FTaskManager* AsyncManager) override;
		virtual void PrepareSingleLoopScopeForEdges(const uint32 StartIndex, const int32 Count) override;
		virtual void ProcessSingleEdge(const int32 EdgeIndex, PCGExGraph::FIndexedEdge& Edge, const int32 LoopIdx, const int32 Count) override;
		virtual void CompleteWork() override;
		virtual void Write() override;
	};

	class FProcessorBatch final : public PCGExClusterMT::TBatch<FProcessor>
	{
		friend class FProcessor;

		FPCGExEdgeDirectionSettings DirectionSettings;

	public:
		FProcessorBatch(FPCGContext* InContext, PCGExData::FPointIO* InVtx, TArrayView<PCGExData::FPointIO*> InEdges):
			TBatch<FProcessor>(InContext, InVtx, InEdges)
		{
		}

		virtual void OnProcessingPreparationComplete() override;
	};
}
