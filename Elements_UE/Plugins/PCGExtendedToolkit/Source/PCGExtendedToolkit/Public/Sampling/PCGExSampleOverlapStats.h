﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"

#include "PCGExPointsProcessor.h"
#include "PCGExSampling.h"
#include "Misc/PCGExDiscardByOverlap.h"

#include "PCGExSampleOverlapStats.generated.h"

#define PCGEX_FOREACH_FIELD_SAMPLEOVERLAPSTATS(MACRO)\
MACRO(OverlapCount, int32)\
MACRO(OverlapSubCount, int32)\
MACRO(RelativeOverlapCount, double)\
MACRO(RelativeOverlapSubCount, double)

namespace PCGExSampleOverlapStats
{
	struct FOverlapStats;
	struct FOverlap;
	class FProcessor;
}

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural), Category="PCGEx|Misc")
class /*PCGEXTENDEDTOOLKIT_API*/ UPCGExSampleOverlapStatsSettings : public UPCGExPointsProcessorSettings
{
	GENERATED_BODY()

public:
	//~Begin UPCGSettings
#if WITH_EDITOR
	PCGEX_NODE_INFOS(SampleOverlapStats, "Sample : Overlap Stats", "Sample & write per-point overlap stats between entire point data.");
	virtual FLinearColor GetNodeTitleColor() const override { return GetDefault<UPCGExGlobalSettings>()->NodeColorSampler; }
#endif

protected:
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings

	//~Begin UPCGExPointsProcessorSettings
public:
	virtual PCGExData::EInit GetMainOutputInitMode() const override;
	PCGEX_NODE_POINT_FILTER(PCGExPointFilter::SourcePointFiltersLabel, "Filters used to know whether a point should be considered for overlap or not.", PCGExFactories::PointFilters, false)
	//~End UPCGExPointsProcessorSettings

public:
	/** Point bounds to be used to compute overlaps */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable))
	EPCGExPointBoundsSource BoundsSource = EPCGExPointBoundsSource::ScaledBounds;

	/** The minimum amount two sub-points must overlap to be added to the comparison.  The higher, the more "overlap" there must be. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable, ClampMin=0))
	double MinThreshold = 0.1;

	/** How to interpret the min overlap value.  Discrete means distance in world space  Relative means uses percentage (0-1) of the averaged radius. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Settings, meta=(PCG_Overridable))
	EPCGExMeanMeasure ThresholdMeasure = EPCGExMeanMeasure::Relative;

	// Output

	/** Write the unique overlap count to an int32 attribute. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteOverlapCount = false;

	/** Name of the 'int32' attribute to write unique overlap count to. Unique overlap count is the number of time a different point data set overlapped this point. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="Overlap Count", PCG_Overridable, EditCondition="bWriteOverlapCount"))
	FName OverlapCountAttributeName = FName("OverlapCount");


	/** Write the total number of overlaps. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteOverlapSubCount = false;

	/** Name of the 'int32' attribute to write total overlap sub-count to. Total overlap count is the number of time another point overlapped this point. This count can get really high, really fast. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="Overlap Sub Count", PCG_Overridable, EditCondition="bWriteOverlapSubCount"))
	FName OverlapSubCountAttributeName = FName("OverlapSubCount");


	/** Write the relative unique overlap count to an int32 attribute. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteRelativeOverlapCount = false;

	/** Name of the 'int32' attribute to write relative unique overlap count to. Relative Unique overlap count is this collection' OverlapSubCount divided by the max of all collections combined */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="Relative Overlap Count", PCG_Overridable, EditCondition="bWriteRelativeOverlapCount"))
	FName RelativeOverlapCountAttributeName = FName("RelOverlapCount");


	/** Write the total number of overlaps. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bWriteRelativeOverlapSubCount = false;

	/** Name of the 'int32' attribute to write relative total overlap sub-count to. Relative Total overlap count is is this collection' OverlapCount divided by the max of all collections combined */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Output", meta=(DisplayName="Relative Overlap Sub Count", PCG_Overridable, EditCondition="bWriteRelativeOverlapSubCount"))
	FName RelativeOverlapSubCountAttributeName = FName("RelOverlapSubCount");

	//

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Tagging", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bTagIfHasAnyOverlap = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Tagging", meta=(PCG_Overridable, EditCondition="bTagIfHasAnyOverlap"))
	FString HasAnyOverlapTag = TEXT("HasAnyOverlap");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Tagging", meta=(PCG_Overridable, InlineEditConditionToggle))
	bool bTagIfHasNoOverlap = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings|Tagging", meta=(PCG_Overridable, EditCondition="bTagIfHasNoOverlap"))
	FString HasNoOverlapTag = TEXT("HasNoOverlap");

private:
	friend class FPCGExSampleOverlapStatsElement;
};

struct /*PCGEXTENDEDTOOLKIT_API*/ FPCGExSampleOverlapStatsContext final : public FPCGExPointsProcessorContext
{
	friend class FPCGExSampleOverlapStatsElement;
	virtual ~FPCGExSampleOverlapStatsContext() override;

	mutable FRWLock OverlapLock;
	TMap<uint64, PCGExSampleOverlapStats::FOverlap*> OverlapMap;

	PCGExSampleOverlapStats::FOverlap* RegisterOverlap(
		PCGExSampleOverlapStats::FProcessor* InManager,
		PCGExSampleOverlapStats::FProcessor* InManaged,
		const FBox& InIntersection);

	virtual void MTState_PointsCompletingWorkDone() override;

	PCGEX_FOREACH_FIELD_SAMPLEOVERLAPSTATS(PCGEX_OUTPUT_DECL_TOGGLE)

	double SharedOverlapSubCountMax = 0;
	double SharedOverlapCountMax = 0;
};

class /*PCGEXTENDEDTOOLKIT_API*/ FPCGExSampleOverlapStatsElement final : public FPCGExPointsProcessorElement
{
	virtual FPCGContext* Initialize(
		const FPCGDataCollection& InputData,
		TWeakObjectPtr<UPCGComponent> SourceComponent,
		const UPCGNode* Node) override;

protected:
	virtual bool Boot(FPCGExContext* InContext) const override;
	virtual bool ExecuteInternal(FPCGContext* Context) const override;
};

namespace PCGExSampleOverlapStats
{
	class FProcessor;

	struct /*PCGEXTENDEDTOOLKIT_API*/ FOverlapStats
	{
		int32 OverlapCount = 0;
		double OverlapVolume = 0;
		double OverlapVolumeAvg = 0;
		double RelativeOverlapCount = 0;
		double RelativeOverlapVolume = 0;

		FORCEINLINE void Add(const FOverlapStats& Other)
		{
			OverlapCount += Other.OverlapCount;
			OverlapVolume += Other.OverlapVolume;
		}

		FORCEINLINE void Remove(const FOverlapStats& Other)
		{
			OverlapCount -= Other.OverlapCount;
			OverlapVolume -= Other.OverlapVolume;
		}

		FORCEINLINE void Add(const FOverlapStats& Other, const int32 MaxCount, const double MaxVolume)
		{
			Add(Other);
			UpdateRelative(MaxCount, MaxVolume);
		}

		FORCEINLINE void Remove(const FOverlapStats& Other, const int32 MaxCount, const double MaxVolume)
		{
			Remove(Other);
			UpdateRelative(MaxCount, MaxVolume);
		}

		FORCEINLINE void UpdateRelative(const int32 MaxCount, const double MaxVolume)
		{
			OverlapVolumeAvg = OverlapVolume / OverlapCount;
			RelativeOverlapCount = OverlapCount / MaxCount;
			RelativeOverlapVolume = OverlapVolume / MaxVolume;
		}
	};

	struct /*PCGEXTENDEDTOOLKIT_API*/ FOverlap
	{
		uint64 HashID = 0;
		FBox Intersection = FBox(NoInit);
		bool IsValid = true;

		FProcessor* Manager = nullptr;
		FProcessor* Managed = nullptr;

		FOverlapStats Stats;

		FOverlap(FProcessor* InManager, FProcessor* InManaged, const FBox& InIntersection);
		FORCEINLINE FProcessor* GetOther(const FProcessor* InCandidate) const { return Manager == InCandidate ? Managed : Manager; }
	};

	class FProcessor final : public PCGExPointsMT::FPointsProcessor
	{
		friend struct FPCGExSampleOverlapStatsContext;

		const UPCGExSampleOverlapStatsSettings* LocalSettings = nullptr;
		FPCGExSampleOverlapStatsContext* LocalTypedContext = nullptr;

		const TArray<FPCGPoint>* InPoints = nullptr;
		FBox Bounds = FBox(ForceInit);

		using TBoundsOctree = TOctree2<PCGExDiscardByOverlap::FPointBounds*, PCGExDiscardByOverlap::FPointBoundsSemantics>;
		TBoundsOctree* Octree = nullptr;

		TArray<PCGExDiscardByOverlap::FPointBounds*> LocalPointBounds;

		mutable FRWLock RegistrationLock;
		TArray<FOverlap*> Overlaps;
		TArray<FOverlap*> ManagedOverlaps;

		int32 NumPoints = 0;

		TArray<int32> OverlapSubCount;
		TArray<int32> OverlapCount;
		double LocalOverlapSubCountMax = 0;
		double LocalOverlapCountMax = 0;

		int8 bAnyOverlap = 0;

		PCGEX_FOREACH_FIELD_SAMPLEOVERLAPSTATS(PCGEX_OUTPUT_DECL)

	public:
		FOverlapStats Stats;

		explicit FProcessor(PCGExData::FPointIO* InPoints)
			: FPointsProcessor(InPoints)
		{
		}

		FORCEINLINE const FBox& GetBounds() const { return Bounds; }
		FORCEINLINE const TArray<PCGExDiscardByOverlap::FPointBounds*>& GetPointBounds() const { return LocalPointBounds; }
		FORCEINLINE const TBoundsOctree* GetOctree() const { return Octree; }

		//virtual bool IsTrivial() const override { return false; } // Force non-trivial because this shit is expensive

		virtual ~FProcessor() override;

		FORCEINLINE void RegisterPointBounds(const int32 Index, PCGExDiscardByOverlap::FPointBounds* InPointBounds)
		{
			Bounds += InPointBounds->Bounds.GetBox();
			LocalPointBounds[Index] = InPointBounds;
		}

		void RegisterOverlap(FProcessor* InManaged, const FBox& Intersection);

		virtual bool Process(PCGExMT::FTaskManager* AsyncManager) override;
		void ResolveOverlap(const int32 Index);
		void WriteSingleData(const int32 Index);
		virtual void CompleteWork() override;
		virtual void Write() override;
	};
}
