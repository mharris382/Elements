﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#include "Graph/PCGExGraph.h"

#include "PCGExPointsProcessor.h"
#include "PCGExRandom.h"
#include "Graph/PCGExCluster.h"
#include "Graph/Data/PCGExClusterData.h"

bool FPCGExGraphBuilderDetails::IsValid(const PCGExGraph::FSubGraph* InSubgraph) const
{
	if (bRemoveBigClusters)
	{
		if (InSubgraph->Edges.Num() > MaxEdgeCount || InSubgraph->Nodes.Num() > MaxVtxCount) { return false; }
	}

	if (bRemoveSmallClusters)
	{
		if (InSubgraph->Edges.Num() < MinEdgeCount || InSubgraph->Nodes.Num() < MinVtxCount) { return false; }
	}

	return true;
}

namespace PCGExGraph
{
	void FSubGraph::Invalidate(FGraph* InGraph)
	{
		for (const int32 EdgeIndex : Edges) { InGraph->Edges[EdgeIndex].bValid = false; }
		for (const int32 NodeIndex : Nodes) { InGraph->Nodes[NodeIndex].bValid = false; }
	}

	PCGExCluster::FCluster* FSubGraph::CreateCluster(PCGExMT::FTaskManager* AsyncManager) const
	{
		PCGExCluster::FCluster* NewCluster = new PCGExCluster::FCluster();
		NewCluster->VtxIO = VtxIO;
		NewCluster->EdgesIO = EdgesIO;

		NewCluster->BuildFrom(this);
		// Look into the cost of this
		if (AsyncManager) { NewCluster->ExpandEdges(AsyncManager); }
		else { NewCluster->GetExpandedEdges(true); }

		return NewCluster;
	}

	int32 FSubGraph::GetFirstInIOIndex()
	{
		for (const int32 InIOIndex : EdgesInIOIndices) { return InIOIndex; }
		return -1;
	}

	void FGraph::ReserveForEdges(const int32 UpcomingAdditionCount)
	{
		const int32 NewMax = Edges.Num() + UpcomingAdditionCount;
		UniqueEdges.Reserve(NewMax);
		Edges.Reserve(NewMax);
		EdgeMetadata.Reserve(UpcomingAdditionCount);
	}

	bool FGraph::InsertEdgeUnsafe(const int32 A, const int32 B, FIndexedEdge& OutEdge, const int32 IOIndex)
	{
		bool bAlreadyExists;
		const uint64 Hash = PCGEx::H64U(A, B);

		UniqueEdges.Add(Hash, &bAlreadyExists);

		if (bAlreadyExists) { return false; }

		OutEdge = Edges.Emplace_GetRef(Edges.Num(), A, B, -1, IOIndex);

		Nodes[A].Add(OutEdge.EdgeIndex);
		Nodes[B].Add(OutEdge.EdgeIndex);

		return true;
	}

	bool FGraph::InsertEdge(const int32 A, const int32 B, FIndexedEdge& OutEdge, const int32 IOIndex)
	{
		FWriteScopeLock WriteLock(GraphLock);
		return InsertEdgeUnsafe(A, B, OutEdge, IOIndex);
	}

	bool FGraph::InsertEdgeUnsafe(const FIndexedEdge& Edge)
	{
		bool bAlreadyExists;
		const uint64 Hash = Edge.H64U();

		UniqueEdges.Add(Hash, &bAlreadyExists);

		if (bAlreadyExists) { return false; }

		UniqueEdges.Add(Hash);

		FIndexedEdge& NewEdge = Edges.Emplace_GetRef(Edge);
		NewEdge.EdgeIndex = Edges.Num() - 1;

		Nodes[Edge.Start].Add(NewEdge.EdgeIndex);
		Nodes[Edge.End].Add(NewEdge.EdgeIndex);

		return true;
	}

	bool FGraph::InsertEdge(const FIndexedEdge& Edge)
	{
		FWriteScopeLock WriteLock(GraphLock);
		return InsertEdgeUnsafe(Edge);
	}

	void FGraph::InsertEdges(const TArray<uint64>& InEdges, const int32 InIOIndex)
	{
		FWriteScopeLock WriteLock(GraphLock);
		uint32 A;
		uint32 B;
		bool bAlreadyExists;

		for (const uint64& E : InEdges)
		{
			UniqueEdges.Add(E, &bAlreadyExists);
			if (bAlreadyExists) { continue; }

			PCGEx::H64(E, A, B);
			const int32 EdgeIndex = Edges.Emplace(Edges.Num(), A, B);
			Nodes[A].Add(EdgeIndex);
			Nodes[B].Add(EdgeIndex);
			Edges[EdgeIndex].IOIndex = InIOIndex;
		}
	}

	int32 FGraph::InsertEdges(const TArray<FIndexedEdge>& InEdges)
	{
		FWriteScopeLock WriteLock(GraphLock);
		const int32 StartIndex = Edges.Num();
		for (const FIndexedEdge& E : InEdges) { InsertEdgeUnsafe(E); }
		return StartIndex;
	}


	void FGraph::InsertEdgesUnsafe(const TSet<uint64>& InEdges, const int32 InIOIndex)
	{
		uint32 A;
		uint32 B;
		bool bAlreadyExists;
		for (const uint64& E : InEdges)
		{
			UniqueEdges.Add(E, &bAlreadyExists);
			if (bAlreadyExists) { continue; }

			PCGEx::H64(E, A, B);
			const int32 EdgeIndex = Edges.Emplace(Edges.Num(), A, B);
			Nodes[A].Add(EdgeIndex);
			Nodes[B].Add(EdgeIndex);
			Edges[EdgeIndex].IOIndex = InIOIndex;
		}
	}

	void FGraph::InsertEdges(const TSet<uint64>& InEdges, const int32 InIOIndex)
	{
		FWriteScopeLock WriteLock(GraphLock);
		InsertEdgesUnsafe(InEdges, InIOIndex);
	}

	TArrayView<FNode> FGraph::AddNodes(const int32 NumNewNodes)
	{
		const int32 StartIndex = Nodes.Num();
		Nodes.SetNum(StartIndex + NumNewNodes);
		for (int i = 0; i < NumNewNodes; ++i)
		{
			FNode& Node = Nodes[StartIndex + i];
			Node.NodeIndex = Node.PointIndex = StartIndex + i;
			Node.Adjacency.Reserve(NumEdgesReserve);
		}

		return MakeArrayView(Nodes.GetData() + StartIndex, NumNewNodes);
	}


	void FGraph::BuildSubGraphs(const FPCGExGraphBuilderDetails& Limits)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FGraph::BuildSubGraphs);

		TBitArray<> VisitedNodes;
		TBitArray<> VisitedEdges;
		VisitedNodes.Init(false, Nodes.Num());
		VisitedEdges.Init(false, Edges.Num());

		int32 VisitedNum = 0;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			const FNode& CurrentNode = Nodes[i];

			if (VisitedNodes[i]) { continue; }

			VisitedNodes[i] = true;
			VisitedNum++;

			if (!CurrentNode.bValid || CurrentNode.Adjacency.IsEmpty()) { continue; }

			FSubGraph* SubGraph = new FSubGraph();
			SubGraph->ParentGraph = this;

			TArray<int32> Stack;
			Stack.Reserve(Nodes.Num() - VisitedNum);
			Stack.Add(i);

			while (Stack.Num() > 0)
			{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 3
				const int32 NextIndex = Stack.Pop(false);
#else
				const int32 NextIndex = Stack.Pop(EAllowShrinking::No);
#endif
				FNode& Node = Nodes[NextIndex];
				Node.NumExportedEdges = 0;

				for (const int32 E : Node.Adjacency)
				{
					if (VisitedEdges[E]) { continue; }

					VisitedEdges[E] = true;

					const FIndexedEdge& Edge = Edges[E];

					if (!Edge.bValid) { continue; }

					const int32 OtherIndex = Edge.Other(NextIndex);
					if (!Nodes[OtherIndex].bValid) { continue; }

					Node.NumExportedEdges++;
					SubGraph->Add(Edge, this);

					if (!VisitedNodes[OtherIndex]) // Only enqueue if not already visited
					{
						VisitedNodes[OtherIndex] = true;
						VisitedNum++;

						Stack.Add(OtherIndex);
					}
				}
			}


			if (!Limits.IsValid(SubGraph))
			{
				SubGraph->Invalidate(this); // Will invalidate isolated points
				delete SubGraph;
			}
			else
			{
				SubGraphs.Add(SubGraph);
			}
		}
	}

	void FGraph::GetConnectedNodes(const int32 FromIndex, TArray<int32>& OutIndices, const int32 SearchDepth) const
	{
		const int32 NextDepth = SearchDepth - 1;
		const FNode& RootNode = Nodes[FromIndex];

		for (const int32 EdgeIndex : RootNode.Adjacency)
		{
			const FIndexedEdge& Edge = Edges[EdgeIndex];
			if (!Edge.bValid) { continue; }

			int32 OtherIndex = Edge.Other(FromIndex);
			if (OutIndices.Contains(OtherIndex)) { continue; }

			OutIndices.Add(OtherIndex);
			if (NextDepth > 0) { GetConnectedNodes(OtherIndex, OutIndices, NextDepth); }
		}
	}

	void FGraphBuilder::CompileAsync(PCGExMT::FTaskManager* AsyncManager, FGraphMetadataDetails* MetadataDetails)
	{
		if (Graph->Nodes.Num() < GetDefault<UPCGExGlobalSettings>()->SmallClusterSize)
		{
			Compile(AsyncManager, MetadataDetails);
		}
		else
		{
			AsyncManager->Start<PCGExGraphTask::FCompileGraph>(
				-1, PointIO, this, MetadataDetails);
		}
	}

	void FGraphBuilder::Compile(PCGExMT::FTaskManager* AsyncManager, FGraphMetadataDetails* MetadataDetails)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FGraphBuilder::Compile);

		Graph->BuildSubGraphs(*OutputDetails);

		if (Graph->SubGraphs.IsEmpty())
		{
			bCompiledSuccessfully = false;
			return;
		}

		PointIO->CleanupKeys(); //Ensure fresh keys later on

		TArray<FNode>& Nodes = Graph->Nodes;
		TArray<int32> ValidNodes;
		ValidNodes.Reserve(Nodes.Num());

		{
			TRACE_CPUPROFILER_EVENT_SCOPE(FCompileGraph::PrunePoints);

			// Rebuild point list with only the one used
			// to know which are used, we need to prune subgraphs first
			TArray<FPCGPoint>& MutablePoints = PointIO->GetOut()->GetMutablePoints();

			if (!MutablePoints.IsEmpty())
			{
				//Assume points were filled before, and remove them from the current array
				TArray<FPCGPoint> PrunedPoints;
				PrunedPoints.Reserve(MutablePoints.Num());

				for (FNode& Node : Nodes)
				{
					if (!Node.bValid || Node.Adjacency.IsEmpty()) { continue; }
					Node.PointIndex = PrunedPoints.Add(MutablePoints[Node.PointIndex]);
					ValidNodes.Add(Node.NodeIndex);
				}

				PointIO->GetOut()->SetPoints(PrunedPoints);
			}
			else
			{
				const int32 NumMaxNodes = Nodes.Num();
				MutablePoints.Reserve(NumMaxNodes);

				for (FNode& Node : Nodes)
				{
					if (!Node.bValid || Node.Adjacency.IsEmpty()) { continue; }
					Node.PointIndex = MutablePoints.Add(PointIO->GetInPoint(Node.PointIndex));
					ValidNodes.Add(Node.NodeIndex);
				}
			}
		}

		PointIO->InitializeNum(PointIO->GetNum(PCGExData::ESource::Out), true);

		PCGEx::TAttributeWriter<int64>* VtxEndpointWriter = new PCGEx::TAttributeWriter<int64>(Tag_VtxEndpoint, 0, false);

		VtxEndpointWriter->BindAndSetNumUninitialized(PointIO);

		const uint64 BaseGUID = PointIO->GetOut()->UID;
		for (const int32 NodeIndex : ValidNodes)
		{
			const FNode& Node = Nodes[NodeIndex];
			VtxEndpointWriter->Values[Node.PointIndex] = PCGEx::H64(NodeGUID(BaseGUID, Node.PointIndex), Node.NumExportedEdges);
		}

		PCGEX_ASYNC_WRITE_DELETE(AsyncManager, VtxEndpointWriter)

		if (MetadataDetails && !Graph->NodeMetadata.IsEmpty())
		{
#define PCGEX_METADATA(_NAME, _TYPE, _DEFAULT, _ACCESSOR)\
{if(MetadataDetails->bWrite##_NAME){\
PCGEx:: TAttributeWriter<_TYPE>* Writer = MetadataDetails->bWrite##_NAME ? new PCGEx:: TAttributeWriter<_TYPE>(MetadataDetails->_NAME##AttributeName, _DEFAULT, false) : nullptr;\
Writer->BindAndSetNumUninitialized(PointIO);\
		for(const int32 NodeIndex : ValidNodes){\
		PCGExGraph::FGraphNodeMetadata** NodeMeta = Graph->NodeMetadata.Find(NodeIndex);\
		if(NodeMeta){Writer->Values[Nodes[NodeIndex].PointIndex] = (*NodeMeta)->_ACCESSOR; }}\
		PCGEX_ASYNC_WRITE_DELETE(AsyncManager, Writer) }}

			PCGEX_METADATA(Compounded, bool, false, bCompounded)
			PCGEX_METADATA(CompoundSize, int32, 0, CompoundSize)
			PCGEX_METADATA(Intersector, bool, false, IsIntersector())
			PCGEX_METADATA(Crossing, bool, false, IsCrossing())

#undef PCGEX_METADATA
		}

		bCompiledSuccessfully = true;

		PCGEx::TAttributeWriter<int64>* NumClusterIdWriter = new PCGEx::TAttributeWriter<int64>(Tag_ClusterId, -1, false);
		NumClusterIdWriter->BindAndSetNumUninitialized(PointIO);

		// Subgraphs

		TArray<FSubGraph*> SmallSubGraphs;

		int32 SubGraphIndex = 0;
		for (FSubGraph* SubGraph : Graph->SubGraphs)
		{
			PCGExData::FPointIO* EdgeIO;

			if (const int32 IOIndex = SubGraph->GetFirstInIOIndex();
				SourceEdgesIO && SourceEdgesIO
				                 ->
				                 Pairs.IsValidIndex(IOIndex)
			)
			{
				EdgeIO = EdgesIO->Emplace_GetRef<UPCGExClusterEdgesData>(SourceEdgesIO->Pairs[IOIndex], PCGExData::EInit::NewOutput);
			}
			else
			{
				EdgeIO = EdgesIO->Emplace_GetRef<UPCGExClusterEdgesData>(PCGExData::EInit::NewOutput);
			}

			const int64 ClusterId = EdgeIO->GetOut()->UID;
			SubGraph->VtxIO = PointIO;
			SubGraph->EdgesIO = EdgeIO;

			MarkClusterEdges(EdgeIO, PairIdStr);
			PCGExData::WriteMark(EdgeIO->GetOut()->Metadata, Tag_ClusterId, ClusterId);

			if (SubGraph->Edges.Num() < 100)
			{
				SubGraphIndex++;
				SmallSubGraphs.Add(SubGraph);
				if (SmallSubGraphs.Num() >= 256)
				{
					AsyncManager->Start<PCGExGraphTask::FWriteSmallSubGraphEdges>(-1, PointIO, SmallSubGraphs, MetadataDetails);
					SmallSubGraphs.Reset();
				}
			}
			else
			{
				AsyncManager->Start<PCGExGraphTask::FWriteSubGraphEdges>(SubGraphIndex++, PointIO, SubGraph, MetadataDetails);
			}

			//PCGExGraphTask::WriteSubGraphEdges(PointIO->GetOut()->GetPoints(), Graph, SubGraph, MetadataDetails);

			for (const int32 EdgeIndex : SubGraph->Edges)
			{
				const FIndexedEdge& Edge = Graph->Edges[EdgeIndex];
				NumClusterIdWriter->Values[Graph->Nodes[Edge.Start].PointIndex] = ClusterId;
				NumClusterIdWriter->Values[Graph->Nodes[Edge.End].PointIndex] = ClusterId;
			}
		}

		if (!SmallSubGraphs.IsEmpty())
		{
			AsyncManager->Start<PCGExGraphTask::FWriteSmallSubGraphEdges>(-1, PointIO, SmallSubGraphs, MetadataDetails);
			SmallSubGraphs.Reset();
		}

		MarkClusterVtx(PointIO, PairIdStr);
		PCGEX_ASYNC_WRITE_DELETE(AsyncManager, NumClusterIdWriter)
	}

	void FGraphBuilder::Write() const
	{
		EdgesIO->OutputToContext();
	}
}

namespace PCGExGraphTask
{
	void WriteSubGraphEdges(
		PCGExMT::FTaskManager* AsyncManager,
		const UPCGPointData* VtxIO,
		PCGExGraph::FSubGraph* SubGraph,
		const PCGExGraph::FGraphMetadataDetails* MetadataDetails)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FWriteSubGraphEdges::ExecuteTask);

		const TArray<FPCGPoint>& Vertices = VtxIO->GetPoints();

		PCGExGraph::FGraph* Graph = SubGraph->ParentGraph;
		TArray<int32> EdgeDump = SubGraph->Edges.Array();
		const int32 NumEdges = EdgeDump.Num();

		TArray<PCGExGraph::FIndexedEdge>& FlattenedEdges = SubGraph->FlattenedEdges;
		FlattenedEdges.SetNumUninitialized(NumEdges);

		PCGExData::FPointIO* EdgeIO = SubGraph->EdgesIO;
		UPCGMetadata* Metadata = EdgeIO->GetOut()->Metadata;

		TArray<FPCGPoint>& MutablePoints = EdgeIO->GetOut()->GetMutablePoints();
		MutablePoints.SetNum(NumEdges);

		if (EdgeIO->GetIn())
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(FWriteSubGraphEdges::GatherPreExistingPoints);

			// Copy any existing point properties first
			const TArray<FPCGPoint>& InPoints = EdgeIO->GetIn()->GetPoints();
			for (int i = 0; i < NumEdges; ++i)
			{
				PCGExGraph::FIndexedEdge& OE = Graph->Edges[EdgeDump[i]];
				FlattenedEdges[i] = PCGExGraph::FIndexedEdge(i, Graph->Nodes[OE.Start].PointIndex, Graph->Nodes[OE.End].PointIndex, i);
				if (InPoints.IsValidIndex(OE.PointIndex)) { MutablePoints[i] = InPoints[OE.PointIndex]; }
				Metadata->InitializeOnSet(MutablePoints[i].MetadataEntry);
			}
		}
		else
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(FWriteSubGraphEdges::CreatePoints);

			for (int i = 0; i < NumEdges; ++i)
			{
				PCGExGraph::FIndexedEdge& E = Graph->Edges[EdgeDump[i]];
				FlattenedEdges[i] = PCGExGraph::FIndexedEdge(i, Graph->Nodes[E.Start].PointIndex, Graph->Nodes[E.End].PointIndex, i);
				Metadata->InitializeOnSet(MutablePoints[i].MetadataEntry);
			}
		}

		EdgeIO->CreateOutKeys();

		PCGEx::TAttributeWriter<int64>* EdgeEndpoints = new PCGEx::TAttributeWriter<int64>(PCGExGraph::Tag_EdgeEndpoints, -1, false);
		EdgeEndpoints->BindAndSetNumUninitialized(EdgeIO);

		const FVector SeedOffset = FVector(EdgeIO->IOIndex);

		const uint64 BaseGUID = VtxIO->UID;
		for (const PCGExGraph::FIndexedEdge& E : FlattenedEdges)
		{
			FPCGPoint& EdgePt = MutablePoints[E.EdgeIndex];

			EdgeEndpoints->Values[E.EdgeIndex] = PCGEx::H64(PCGExGraph::NodeGUID(BaseGUID, E.Start), PCGExGraph::NodeGUID(BaseGUID, E.End));

			/*
			if (PCGExGraph::FGraphEdgeMetadata** EdgeMetaPtr = Graph->EdgeMetadata.Find(EdgeIndex))
			{
				PCGExGraph::FGraphEdgeMetadata* EdgeMeta = *EdgeMetaPtr;
				//int32 ParentCompoundIndex = PCGExGraph::FGraphEdgeMetadata::GetParentIndex();
				//TODO: Handle edge metadata	
			}
			*/

			if (Graph->bWriteEdgePosition)
			{
				EdgePt.Transform.SetLocation(
					FMath::Lerp(
						Vertices[E.Start].Transform.GetLocation(),
						Vertices[E.End].Transform.GetLocation(),
						Graph->EdgePosition));
			}

			if (EdgePt.Seed == 0 || Graph->bRefreshEdgeSeed) { EdgePt.Seed = PCGExRandom::ComputeSeed(EdgePt, SeedOffset); }
		}

		PCGEX_ASYNC_WRITE_DELETE(AsyncManager, EdgeEndpoints)

		/*
		//TODO : Implement
		if (MetadataDetails &&
			!Graph->EdgeMetadata.IsEmpty() &&
			!Graph->NodeMetadata.IsEmpty())
		{
			if (MetadataDetails->bFlagCrossing)
			{
				TRACE_CPUPROFILER_EVENT_SCOPE(FWriteSubGraphEdges::FlagCrossing);

				// Need to go through each point and add flags matching edges
				for (const int32 NodeIndex : SubGraph->Nodes)
				{
					PCGExGraph::FNode Node = Graph->Nodes[NodeIndex];
					PCGExGraph::FGraphNodeMetadata** NodeMetaPtr = Graph->NodeMetadata.Find(NodeIndex);

					if (!NodeMetaPtr || (*NodeMetaPtr)->Type != EPCGExIntersectionType::EdgeEdge)
					{
					}
				}
			}
		}
		*/

		if (GetDefault<UPCGExGlobalSettings>()->bCacheClusters && Graph->bBuildClusters)
		{
			if (UPCGExClusterEdgesData* ClusterEdgesData = Cast<UPCGExClusterEdgesData>(EdgeIO->GetOut()))
			{
				if (NumEdges < 100)
				{
					PCGExCluster::FCluster* Cluster = SubGraph->CreateCluster(nullptr);
					if (Graph->bExpandClusters) { Cluster->GetExpandedNodes(true); }

					ClusterEdgesData->SetBoundCluster(Cluster, true);
				}
				else { AsyncManager->Start<FWriteSubGraphCluster>(-1, nullptr, SubGraph); }
			}
		}
	}

	bool FWriteSubGraphEdges::ExecuteTask()
	{
		WriteSubGraphEdges(Manager, PointIO->GetOut(), SubGraph, MetadataDetails);
		return true;
	}

	bool FWriteSmallSubGraphEdges::ExecuteTask()
	{
		for (PCGExGraph::FSubGraph* SubGraph : SubGraphs)
		{
			WriteSubGraphEdges(Manager, PointIO->GetOut(), SubGraph, MetadataDetails);
		}

		return true;
	}

	bool FWriteSubGraphCluster::ExecuteTask()
	{
		UPCGExClusterEdgesData* ClusterEdgesData = Cast<UPCGExClusterEdgesData>(SubGraph->EdgesIO->GetOut());
		PCGExCluster::FCluster* Cluster = SubGraph->CreateCluster(Manager);
		if (SubGraph->ParentGraph->bExpandClusters) { Cluster->ExpandNodes(Manager); }

		ClusterEdgesData->SetBoundCluster(Cluster, true);
		return true;
	}

	bool FCompileGraph::ExecuteTask()
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(FCompileGraph::ExecuteTask);

		Builder->Compile(Manager, MetadataDetails);
		return true;
	}

	bool FCopyGraphToPoint::ExecuteTask()
	{
		if (!GraphBuilder->bCompiledSuccessfully) { return false; }

		PCGExData::FPointIO* VtxDupe = VtxCollection->Emplace_GetRef(GraphBuilder->PointIO->GetOut(), PCGExData::EInit::DuplicateInput);
		VtxDupe->IOIndex = TaskIndex;

		FString OutId;
		PCGExGraph::SetClusterVtx(VtxDupe, OutId);

		InternalStart<PCGExGeoTasks::FTransformPointIO>(TaskIndex, PointIO, VtxDupe, TransformDetails);

		for (const PCGExData::FPointIO* Edges : GraphBuilder->EdgesIO->Pairs)
		{
			PCGExData::FPointIO* EdgeDupe = EdgeCollection->Emplace_GetRef(Edges->GetOut(), PCGExData::EInit::DuplicateInput);
			EdgeDupe->IOIndex = TaskIndex;
			PCGExGraph::MarkClusterEdges(EdgeDupe, OutId);

			InternalStart<PCGExGeoTasks::FTransformPointIO>(TaskIndex, PointIO, EdgeDupe, TransformDetails);
		}

		// TODO : Copy & Transform cluster as well for a big perf boost

		return true;
	}
}
