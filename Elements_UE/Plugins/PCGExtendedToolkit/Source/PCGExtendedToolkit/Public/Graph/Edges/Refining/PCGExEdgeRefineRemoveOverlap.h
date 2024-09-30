﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGExEdgeRefineOperation.h"
#include "Graph/PCGExCluster.h"
#include "PCGExEdgeRefineRemoveOverlap.generated.h"

UENUM(BlueprintType, meta=(DisplayName="[PCGEx] Edge Overlap Pick"))
enum class EPCGExEdgeOverlapPick : uint8
{
	Shortest = 0 UMETA(DisplayName = "Shortest", ToolTip="Keep the shortest edge"),
	Longest  = 1 UMETA(DisplayName = "Longest", ToolTip="Keep the longest edge"),
};

/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType, meta=(DisplayName="Remove Overlap"))
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExEdgeRemoveOverlap : public UPCGExEdgeRefineOperation
{
	GENERATED_BODY()

public:
	virtual bool RequiresIndividualEdgeProcessing() override { return true; }
	virtual bool RequiresEdgeOctree() override { return true; }

	virtual void PrepareForCluster(PCGExCluster::FCluster* InCluster, PCGExHeuristics::THeuristicsHandler* InHeuristics) override
	{
		Super::PrepareForCluster(InCluster, InHeuristics);
		MinDot = bUseMinAngle ? PCGExMath::DegreesToDot(MinAngle) : 1;
		MaxDot = bUseMaxAngle ? PCGExMath::DegreesToDot(MaxAngle) : -1;
		ToleranceSquared = Tolerance * Tolerance;
		Cluster->GetExpandedEdges(true); // Let's hope it was cached ^_^
	}

	virtual void CopySettingsFrom(const UPCGExOperation* Other) override
	{
		Super::CopySettingsFrom(Other);
		if (const UPCGExEdgeRemoveOverlap* TypedOther = Cast<UPCGExEdgeRemoveOverlap>(Other))
		{
			Keep = TypedOther->Keep;
			Tolerance = TypedOther->Tolerance;
			bUseMinAngle = TypedOther->bUseMinAngle;
			MinAngle = TypedOther->MinAngle;
			bUseMaxAngle = TypedOther->bUseMaxAngle;
			MaxAngle = TypedOther->MaxAngle;
		}
	}

	virtual void ProcessEdge(PCGExGraph::FIndexedEdge& Edge) override
	{
		const PCGExCluster::FExpandedEdge* EEdge = *(Cluster->ExpandedEdges->GetData() + Edge.EdgeIndex);
		const double Length = EEdge->GetEdgeLengthSquared(Cluster);

		auto ProcessOverlap = [&](const PCGExCluster::FClusterItemRef& ItemRef)
		{
			//if (!Edge.bValid) { return false; }

			const PCGExCluster::FExpandedEdge* OtherEEdge = *(Cluster->ExpandedEdges->GetData() + ItemRef.ItemIndex);

			if (EEdge == OtherEEdge ||
				EEdge->Start == OtherEEdge->Start || EEdge->Start == OtherEEdge->End ||
				EEdge->End == OtherEEdge->End || EEdge->End == OtherEEdge->Start) { return true; }


			if (bUseMinAngle || bUseMaxAngle)
			{
				const double Dot = FMath::Abs(FVector::DotProduct(Cluster->GetDir(*EEdge->Start, *EEdge->End), Cluster->GetDir(*OtherEEdge->Start, *OtherEEdge->End)));
				if (!(Dot >= MaxDot && Dot <= MinDot)) { return true; }
			}

			const double OtherLength = OtherEEdge->GetEdgeLengthSquared(Cluster);

			FVector A;
			FVector B;
			if (Cluster->EdgeDistToEdgeSquared(EEdge->GetNodes(), OtherEEdge->GetNodes(), A, B) >= ToleranceSquared) { return true; }

			// Overlap!
			if (Keep == EPCGExEdgeOverlapPick::Longest)
			{
				if (OtherLength > Length)
				{
					FPlatformAtomics::InterlockedExchange(&Edge.bValid, 0);
					return false;
				}
			}
			else
			{
				if (OtherLength < Length)
				{
					FPlatformAtomics::InterlockedExchange(&Edge.bValid, 0);
					return false;
				}
			}

			return true;
		};

		Cluster->EdgeOctree->FindFirstElementWithBoundsTest(FBoxCenterAndExtent(EEdge->Bounds), ProcessOverlap);
	}

	//virtual void Process() override;

	/** Which edge to keep when doing comparison. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta = (PCG_Overridable))
	EPCGExEdgeOverlapPick Keep = EPCGExEdgeOverlapPick::Longest;

	/** Distance at which two edges are considered intersecting. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, ClampMin=0))
	double Tolerance = 0.001;
	double ToleranceSquared = 0.001;

	/** . */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bUseMinAngle = true;

	/** Min angle. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, EditCondition="bUseMinAngle", Units="Degrees", ClampMin=0, ClampMax=90))
	double MinAngle = 0;
	double MinDot = 1;

	/** . */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bUseMaxAngle = true;

	/** Maximum angle. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, EditCondition="bUseMaxAngle", Units="Degrees", ClampMin=0, ClampMax=90))
	double MaxAngle = 90;
	double MaxDot = -1;
};
