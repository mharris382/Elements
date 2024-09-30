﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#include "Graph/Pathfinding/Heuristics/PCGExHeuristicInertia.h"

void UPCGExHeuristicInertia::PrepareForCluster(const PCGExCluster::FCluster* InCluster)
{
	if (bInvert)
	{
		OutMin = 0;
		OutMax = 1;
	}
	else
	{
		OutMin = 1;
		OutMax = 0;
	}
	Super::PrepareForCluster(InCluster);
}

UPCGExHeuristicOperation* UPCGExHeuristicsFactoryInertia::CreateOperation() const
{
	PCGEX_NEW_TRANSIENT(UPCGExHeuristicInertia, NewOperation)
	PCGEX_FORWARD_HEURISTIC_CONFIG
	NewOperation->GlobalInertiaScore = Config.GlobalInertiaScore;
	NewOperation->FallbackInertiaScore = Config.FallbackInertiaScore;
	return NewOperation;
}

UPCGExParamFactoryBase* UPCGExHeuristicsInertiaProviderSettings::CreateFactory(FPCGExContext* InContext, UPCGExParamFactoryBase* InFactory) const
{
	UPCGExHeuristicsFactoryInertia* NewFactory = NewObject<UPCGExHeuristicsFactoryInertia>();
	PCGEX_FORWARD_HEURISTIC_FACTORY
	return Super::CreateFactory(InContext, NewFactory);
}

#if WITH_EDITOR
FString UPCGExHeuristicsInertiaProviderSettings::GetDisplayName() const
{
	return GetDefaultNodeName().ToString()
		+ TEXT(" @ ")
		+ FString::Printf(TEXT("%.3f"), (static_cast<int32>(1000 * Config.WeightFactor) / 1000.0));
}
#endif
