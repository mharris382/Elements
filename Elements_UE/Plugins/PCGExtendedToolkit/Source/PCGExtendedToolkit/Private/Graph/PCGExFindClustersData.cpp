﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#include "Graph/PCGExFindClustersData.h"
#include "Graph/PCGExGraph.h"

#define LOCTEXT_NAMESPACE "PCGExFindClustersDataElement"
#define PCGEX_NAMESPACE BuildCustomGraph

TArray<FPCGPinProperties> UPCGExFindClustersDataSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	PCGEX_PIN_ANY(GetMainInputLabel(), "The point data to be processed.", Required, {})
	if (SearchMode != EPCGExClusterDataSearchMode::All)
	{
		PCGEX_PIN_POINT(GetSearchOutputLabel(), "The search data to match against.", Required, {})
	}
	return PinProperties;
}

TArray<FPCGPinProperties> UPCGExFindClustersDataSettings::OutputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties = Super::OutputPinProperties();
	PCGEX_PIN_POINTS(PCGExGraph::OutputEdgesLabel, "Point data representing edges.", Required, {})
	return PinProperties;
}

PCGExData::EInit UPCGExFindClustersDataSettings::GetMainOutputInitMode() const { return PCGExData::EInit::NoOutput; }

PCGEX_INITIALIZE_ELEMENT(FindClustersData)

FPCGExFindClustersDataContext::~FPCGExFindClustersDataContext()
{
	PCGEX_TERMINATE_ASYNC

	PCGEX_DELETE(SearchKeyIO)
	PCGEX_DELETE(MainEdges)
}

bool FPCGExFindClustersDataElement::Boot(FPCGExContext* InContext) const
{
	if (!FPCGExPointsProcessorElement::Boot(InContext)) { return false; }

	PCGEX_CONTEXT_AND_SETTINGS(FindClustersData)

	Context->MainEdges = new PCGExData::FPointIOCollection(Context);
	Context->MainEdges->DefaultOutputLabel = PCGExGraph::OutputEdgesLabel;

	if (Settings->SearchMode != EPCGExClusterDataSearchMode::All)
	{
		Context->SearchKeyIO = PCGExData::TryGetSingleInput(Context, Settings->GetSearchOutputLabel(), true);
		if (!Context->SearchKeyIO)
		{
			PCGE_LOG_C(Error, GraphAndLog, InContext, FTEXT("Invalid reference input."));
			return false;
		}

		if (Settings->SearchMode == EPCGExClusterDataSearchMode::EdgesFromVtx)
		{
			if (!Context->SearchKeyIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExVtx))
			{
				PCGE_LOG_C(Error, GraphAndLog, InContext, FTEXT("Invalid reference input (not a Vtx group)."));
				return false;
			}
		}
		else if (Settings->SearchMode == EPCGExClusterDataSearchMode::VtxFromEdges)
		{
			if (!Context->SearchKeyIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExEdges))
			{
				PCGE_LOG_C(Error, GraphAndLog, InContext, FTEXT("Invalid reference input. (not an Edges group)"));
				return false;
			}
		}

		Context->SearchKeyIO->Tags->GetValue(PCGExGraph::TagStr_ClusterPair, Context->SearchKey);
	}

	return true;
}


bool FPCGExFindClustersDataElement::ExecuteInternal(FPCGContext* InContext) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FPCGExFindClustersDataElement::Execute);

	PCGEX_CONTEXT_AND_SETTINGS(FindClustersData)

	if (!Boot(Context)) { return true; }

	if (Settings->SearchMode == EPCGExClusterDataSearchMode::EdgesFromVtx)
	{
		// We have a single Vtx input, find matching edges
		FString OtherKey = TEXT("");
		for (int i = 0; i < Context->MainPoints->Pairs.Num(); i++)
		{
			PCGExData::FPointIO* InputIO = Context->MainPoints->Pairs[i];
			if (!InputIO->Tags->GetValue(PCGExGraph::TagStr_ClusterPair, OtherKey) || OtherKey != Context->SearchKey) { continue; }
			if (!InputIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExEdges)) { continue; }

			// Remove found edges from main points
			Context->MainPoints->Pairs.RemoveAt(i);
			i--;

			// Add to main edges & forward
			Context->MainEdges->AddUnsafe(InputIO);
			InputIO->InitializeOutput(PCGExData::EInit::Forward);
		}

		if (Context->MainEdges->IsEmpty())
		{
			PCGE_LOG_C(Error, GraphAndLog, InContext, FTEXT("Could not find any match."));
			return true;
		}

		// Add vtx key to main output & forward
		Context->MainPoints->AddUnsafe(Context->SearchKeyIO);
		Context->SearchKeyIO->InitializeOutput(PCGExData::EInit::Forward);
		Context->SearchKeyIO = nullptr;

		Context->MainPoints->OutputToContext();
		Context->MainEdges->OutputToContext();

		Context->Done();

		return Context->TryComplete();
	}

	if (Settings->SearchMode == EPCGExClusterDataSearchMode::VtxFromEdges)
	{
		// We have a single Edge input, find (first) matching Vtx
		bool bFoundMatch = false;
		FString OtherKey = TEXT("");
		for (int i = 0; i < Context->MainPoints->Pairs.Num(); i++)
		{
			PCGExData::FPointIO* InputIO = Context->MainPoints->Pairs[i];
			if (!InputIO->Tags->GetValue(PCGExGraph::TagStr_ClusterPair, OtherKey) || OtherKey != Context->SearchKey) { continue; }
			if (!InputIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExVtx)) { continue; }

			// Output vtx
			InputIO->InitializeOutput(PCGExData::EInit::Forward);
			bFoundMatch = true;
			break;
		}

		if (!bFoundMatch)
		{
			PCGE_LOG_C(Error, GraphAndLog, InContext, FTEXT("Could not find any match."));
			return true;
		}

		// Move edge key to edges output and forward
		Context->MainEdges->AddUnsafe(Context->SearchKeyIO);
		Context->SearchKeyIO->InitializeOutput(PCGExData::EInit::Forward);
		Context->SearchKeyIO = nullptr;

		Context->MainPoints->OutputToContext();
		Context->MainEdges->OutputToContext();

		Context->Done();

		return Context->TryComplete();
	}

	PCGExData::FPointIOTaggedDictionary* InputDictionary = new PCGExData::FPointIOTaggedDictionary(PCGExGraph::TagStr_ClusterPair);

	TArray<PCGExData::FPointIO*> TaggedVtx;
	TArray<PCGExData::FPointIO*> TaggedEdges;

	for (PCGExData::FPointIO* MainIO : Context->MainPoints->Pairs)
	{
		// Vtx ?

		if (MainIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExVtx))
		{
			if (MainIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExEdges))
			{
				if (!Settings->bSkipTrivialWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("Uh oh, a data is marked as both Vtx and Edges -- it will be ignored for safety.")); }
				continue;
			}

			if (!PCGExGraph::IsPointDataVtxReady(MainIO->GetIn()->Metadata))
			{
				if (!Settings->bSkipTrivialWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("A Vtx-tagged input has no metadata and will be discarded.")); }
				continue;
			}

			TaggedVtx.Add(MainIO);
			continue;
		}

		// Edge ?

		if (MainIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExEdges))
		{
			if (MainIO->Tags->IsTagged(PCGExGraph::TagStr_PCGExVtx))
			{
				if (!Settings->bSkipTrivialWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("Uh oh, a data is marked as both Vtx and Edges -- it will be ignored for safety.")); }
				continue;
			}

			if (!PCGExGraph::IsPointDataEdgeReady(MainIO->GetIn()->Metadata))
			{
				if (!Settings->bSkipTrivialWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("An Edges-tagged input has no edge metadata and will be discarded.")); }
				continue;
			}

			TaggedEdges.Add(MainIO);
			continue;
		}

		if (!Settings->bSkipTrivialWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("An input data is neither tagged Vtx or Edges and will be ignored.")); }
	}

	for (PCGExData::FPointIO* Vtx : TaggedVtx)
	{
		if (!InputDictionary->CreateKey(*Vtx))
		{
			if (!Settings->bSkipImportantWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("At least two Vtx inputs share the same PCGEx/Cluster tag. Only one will be processed.")); }
			Vtx->Disable();
		}
	}

	for (PCGExData::FPointIO* Edges : TaggedEdges)
	{
		if (!InputDictionary->TryAddEntry(*Edges))
		{
			if (!Settings->bSkipImportantWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("Some input edges have no associated vtx.")); }
		}
	}

	for (PCGExData::FPointIO* Vtx : TaggedVtx)
	{
		if (!Vtx->IsEnabled()) { continue; }

		PCGExData::FPointIOTaggedEntries* EdgesEntries;

		if (FString CurrentPairId;
			Vtx->Tags->GetValue(PCGExGraph::TagStr_ClusterPair, CurrentPairId))
		{
			EdgesEntries = InputDictionary->GetEntries(CurrentPairId);
			if (!EdgesEntries || EdgesEntries->Entries.IsEmpty()) { EdgesEntries = nullptr; }
		}
		else { EdgesEntries = nullptr; }

		if (EdgesEntries)
		{
			Vtx->InitializeOutput(PCGExData::EInit::Forward);
			Vtx->DefaultOutputLabel = PCGExGraph::OutputVerticesLabel;

			for (PCGExData::FPointIO* ValidEdges : EdgesEntries->Entries)
			{
				Context->MainPoints->Pairs.Remove(ValidEdges);
				Context->MainEdges->Pairs.Add(ValidEdges);
				ValidEdges->DefaultOutputLabel = PCGExGraph::OutputEdgesLabel;

				ValidEdges->InitializeOutput(PCGExData::EInit::Forward);
			}
		}
		else
		{
			if (!Settings->bSkipImportantWarnings) { PCGE_LOG(Warning, GraphAndLog, FTEXT("Some input vtx have no associated edges.")); }
		}
	}

	PCGEX_DELETE(InputDictionary)

	Context->MainPoints->OutputToContext();
	Context->MainEdges->OutputToContext();

	Context->Done();

	return Context->TryComplete();
}

#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE
