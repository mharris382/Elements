﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#pragma once

#include "CoreMinimal.h"
#include "PCGEx.h"
#include "Data/PCGPointData.h"
#include "Graph/PCGExEdge.h"

namespace PCGExGeo
{
	struct /*PCGEXTENDEDTOOLKIT_API*/ FCut
	{
		FVector Position = FVector::ZeroVector;
		FVector Normal = FVector::ZeroVector;
		int32 BoxIndex = -1;
		bool bIsInside = false;

		FCut(const FVector& InPosition, const FVector& InNormal, const int32 InBoxIndex):
			Position(InPosition), Normal(InNormal), BoxIndex(InBoxIndex)
		{
		}
	};

	struct /*PCGEXTENDEDTOOLKIT_API*/ FSample
	{
		FVector Distances = FVector::ZeroVector;
		FVector UVW = FVector::ZeroVector;
		double Weight = 0;
		int32 BoxIndex = -1;
		bool bIsInside = false;

		FSample()
		{
		}

		FSample(const FVector& InDistances, const int32 InBoxIndex, const bool IsInside):
			Distances(InDistances), BoxIndex(InBoxIndex), bIsInside(IsInside)
		{
		}
	};

	struct /*PCGEXTENDEDTOOLKIT_API*/ FIntersections
	{
		TArray<FCut> Cuts;
		FVector StartPosition = FVector::ZeroVector;
		FVector EndPosition = FVector::ZeroVector;
		int32 Start = -1;
		int32 End = -1;

		FIntersections(const FVector& InStartPosition, const FVector& InEndPosition, const int32 InStart, const int32 InEnd):
			StartPosition(InStartPosition), EndPosition(InEndPosition), Start(InStart), End(InEnd)
		{
		}

		FIntersections(const int32 InStart, const int32 InEnd):
			Start(InStart), End(InEnd)
		{
		}

		FORCEINLINE uint64 GetKey() const { return PCGEx::H64U(Start, End); }

		void Sort()
		{
			Cuts.Sort(
				[&](const FCut& A, const FCut& B)
				{
					return FVector::DistSquared(StartPosition, B.Position) < FVector::DistSquared(StartPosition, B.Position);
				});
		}

		void SortAndDedupe()
		{
			Sort();

			if (Cuts.IsEmpty() || Cuts.Num() < 2) { return; }

			FVector LastPos = Cuts[0].Position;

			for (int i = 1; i < Cuts.Num(); ++i)
			{
				FVector Pos = (Cuts.GetData() + i)->Position;
				if (Pos == LastPos)
				{
					Cuts.RemoveAt(i);
					i--;
				}
				LastPos = Pos;
			}
		}

		FBoxCenterAndExtent GetBoxCenterAndExtent() const
		{
			FBox Box = FBox(ForceInit);
			Box += StartPosition;
			Box += EndPosition;
			return FBoxCenterAndExtent(Box);
		}

		void Insert(const FVector& Position, const FVector& Normal, const int32 Index)
		{
			Cuts.Emplace(Position, Normal, Index);
		}
	};

	class /*PCGEXTENDEDTOOLKIT_API*/ FSegmentation
	{
		mutable FRWLock IntersectionsLock;

	public:
		TMap<uint64, FIntersections*> IntersectionsMap;
		TArray<FIntersections*> IntersectionsList;

		FSegmentation()
		{
		}

		~FSegmentation()
		{
			PCGEX_DELETE_TMAP(IntersectionsMap, uint64)
			PCGEX_DELETE_TARRAY(IntersectionsList)
		}

		int32 GetNum() const { return IntersectionsMap.Num(); }

		int32 GetNumCuts() const
		{
			TArray<uint64> Keys;
			IntersectionsMap.GetKeys(Keys);
			int32 Sum = 0;
			for (const uint64 Key : Keys) { Sum += IntersectionsMap[Key]->Cuts.Num(); }
			Keys.Empty();
			return Sum;
		}

		void ReduceToArray()
		{
			PCGEX_SET_NUM_UNINITIALIZED(IntersectionsList, IntersectionsMap.Num())
			int32 Index = 0;
			for (const TPair<uint64, FIntersections*>& Pair : IntersectionsMap) { IntersectionsList[Index++] = Pair.Value; }
			IntersectionsMap.Empty();
		}

		FORCEINLINE FIntersections* Find(const uint64 Key)
		{
			{
				FReadScopeLock ReadScopeLock(IntersectionsLock);
				if (FIntersections** ExistingIntersection = IntersectionsMap.Find(Key)) { return *ExistingIntersection; }
			}

			return nullptr;
		}

		void Insert(FIntersections* InIntersections)
		{
			FWriteScopeLock WriteScopeLock(IntersectionsLock);
			IntersectionsMap.Add(InIntersections->GetKey(), InIntersections);
		}

		FIntersections* GetOrCreate(const int32 Start, const int32 End)
		{
			const uint64 HID = PCGEx::H64U(Start, End);
			if (FIntersections* ExistingIntersection = Find(HID)) { return ExistingIntersection; }

			FIntersections* NewIntersections = new FIntersections(Start, End);
			Insert(NewIntersections);

			return NewIntersections;
		}
	};

	struct /*PCGEXTENDEDTOOLKIT_API*/ FPointBox
	{
		FTransform Transform;
		FBoxSphereBounds Sphere;
		FVector Extents;
		FBox Box;
		FBox EpsilonBox;
		double Len;
		int32 Index;

		explicit FPointBox(const FPCGPoint& InPoint, const int32 InIndex, const EPCGExPointBoundsSource BoundsSource, double Epsilon = DBL_EPSILON):
			Transform(FTransform(InPoint.Transform.GetRotation(), InPoint.Transform.GetLocation(), FVector::One())),
			Index(InIndex)
		{
			const FBox PointBox = PCGExMath::GetLocalBounds(InPoint, BoundsSource);
			Extents = PointBox.GetExtent();
			Len = Extents.Length();

			Box = FBox(Extents * -1, Extents);

			EpsilonBox = Box.ExpandBy(-Epsilon);
			Sphere = FBoxSphereBounds(Transform.GetLocation(), FVector(Len), Len);
		}

		FORCEINLINE bool Contains(const FVector& Position) const { return Box.IsInside(Transform.InverseTransformPosition(Position)); }
		FORCEINLINE bool ContainsMinusEpsilon(const FVector& Position) const { return EpsilonBox.IsInside(Transform.InverseTransformPosition(Position)); }

		FORCEINLINE bool Intersect(const FPCGPoint& Point, const EPCGExPointBoundsSource BoundsSource) const
		{
			const FBox LocalBox = PCGExMath::GetLocalBounds(Point, BoundsSource).TransformBy(Point.Transform).InverseTransformBy(Transform);
			return Box.Intersect(LocalBox);
		}

		FORCEINLINE bool Contains(const FPCGPoint& Point, const EPCGExPointBoundsSource BoundsSource) const
		{
			const FBox LocalBox = PCGExMath::GetLocalBounds(Point, BoundsSource).TransformBy(Point.Transform).InverseTransformBy(Transform);
			return Box.IsInside(LocalBox);
		}

		FORCEINLINE bool ContainsOrIntersect(const FPCGPoint& Point, const EPCGExPointBoundsSource BoundsSource, bool& bContains, bool& bIntersects) const
		{
			const FBox LocalBox = PCGExMath::GetLocalBounds(Point, BoundsSource).TransformBy(Point.Transform).InverseTransformBy(Transform);
			bContains = Box.IsInside(LocalBox);
			bIntersects = Box.Intersect(LocalBox);
			return bContains || bIntersects;
		}

		FORCEINLINE void Sample(const FVector& Position, FSample& OutSample) const
		{
			const FVector LocalPosition = Transform.InverseTransformPosition(Position);
			OutSample.bIsInside = Box.IsInside(LocalPosition);
			OutSample.Distances = LocalPosition;
			OutSample.BoxIndex = Index;
			OutSample.UVW = FVector(
				LocalPosition.X / Extents.X,
				LocalPosition.Y / Extents.Y,
				LocalPosition.Z / Extents.Z);
			OutSample.Weight = 1 - ((
				(FMath::Clamp(FMath::Abs(OutSample.UVW.X), 0, Extents.X) / Extents.X) +
				(FMath::Clamp(FMath::Abs(OutSample.UVW.Y), 0, Extents.Y) / Extents.Y) +
				(FMath::Clamp(FMath::Abs(OutSample.UVW.Z), 0, Extents.Z) / Extents.Z)) / 3);
		}

		FORCEINLINE void Sample(const FPCGPoint& Point, FSample& OutSample) const
		{
			Sample(Point.Transform.GetLocation(), OutSample);
		}


		bool ProcessIntersections(FIntersections* InIntersections) const
		{
			FVector OutIntersection1 = FVector::ZeroVector;
			FVector OutIntersection2 = FVector::ZeroVector;
			FVector OutHitNormal1 = FVector::ZeroVector;
			FVector OutHitNormal2 = FVector::ZeroVector;
			bool bIsIntersection2Valid = false;
			bool bInverseDir = false;
			if (SegmentIntersection(
				InIntersections->StartPosition, InIntersections->EndPosition,
				OutIntersection1, OutIntersection2, bIsIntersection2Valid,
				OutHitNormal1, OutHitNormal2, bInverseDir))
			{
				InIntersections->Insert(OutIntersection1, OutHitNormal1, Index);
				if (bIsIntersection2Valid) { InIntersections->Insert(OutIntersection2, OutHitNormal2, Index); }
				return true;
			}
			return false;
		}

		bool SegmentIntersection(
			const FVector& Start,
			const FVector& End,
			FVector& OutIntersection1,
			FVector& OutIntersection2,
			bool& bIsI2Valid,
			FVector& OutHitNormal1,
			FVector& OutHitNormal2,
			bool& bInverseDir) const
		{
			const FVector LocalStart = Transform.InverseTransformPosition(Start);
			const FVector LocalEnd = Transform.InverseTransformPosition(End);

			const bool bIsStartInside = Box.IsInside(LocalStart);
			const bool bIsEndInside = Box.IsInside(LocalEnd);

			bIsI2Valid = false;
			bInverseDir = false;

			if (bIsStartInside && bIsEndInside) { return false; }

			FVector HitLocation;
			FVector HitNormal;
			float HitTime;

			bool bHasValidIntersection = false;

			if (bIsEndInside)
			{
				if (FMath::LineExtentBoxIntersection(Box, LocalStart, LocalEnd, FVector::ZeroVector, HitLocation, HitNormal, HitTime))
				{
					OutIntersection1 = Transform.TransformPosition(HitLocation);
					OutHitNormal1 = Transform.TransformVector(HitNormal);
					return OutIntersection1 != Start && OutIntersection1 != End;
				}

				return false;
			}

			if (bIsStartInside)
			{
				if (FMath::LineExtentBoxIntersection(Box, LocalEnd, LocalStart, FVector::ZeroVector, HitLocation, HitNormal, HitTime))
				{
					OutIntersection1 = Transform.TransformPosition(HitLocation);
					OutHitNormal1 = Transform.TransformVector(HitNormal);
					bInverseDir = true;
					return OutIntersection1 != Start && OutIntersection1 != End;
				}

				return false;
			}

			if (FMath::LineExtentBoxIntersection(Box, LocalStart, LocalEnd, FVector::ZeroVector, HitLocation, HitNormal, HitTime))
			{
				OutIntersection1 = Transform.TransformPosition(HitLocation);
				OutHitNormal1 = Transform.TransformVector(HitNormal);
				bHasValidIntersection = OutIntersection1 != Start && OutIntersection1 != End;
			}

			if (FMath::LineExtentBoxIntersection(Box, LocalEnd, LocalStart, FVector::ZeroVector, HitLocation, HitNormal, HitTime))
			{
				if (!bHasValidIntersection)
				{
					OutIntersection1 = Transform.TransformPosition(HitLocation);
					OutHitNormal1 = Transform.TransformVector(HitNormal);
					bInverseDir = true;
					bHasValidIntersection = OutIntersection1 != Start && OutIntersection1 != End;
				}
				else
				{
					OutIntersection2 = Transform.TransformPosition(HitLocation);
					OutHitNormal2 = Transform.TransformVector(HitNormal);
					bIsI2Valid = OutIntersection1 != OutIntersection2 && (OutIntersection2 != Start && OutIntersection2 != End);
				}

				bHasValidIntersection = bHasValidIntersection || bIsI2Valid;
			}

			return bHasValidIntersection;
		}
	};

	struct /*PCGEXTENDEDTOOLKIT_API*/ FPointBoxSemantics
	{
		enum { MaxElementsPerLeaf = 16 };

		enum { MinInclusiveElementsPerNode = 7 };

		enum { MaxNodeDepth = 12 };

		using ElementAllocator = TInlineAllocator<MaxElementsPerLeaf>;

		FORCEINLINE static const FBoxSphereBounds& GetBoundingBox(const FPointBox* InPoint)
		{
			return InPoint->Sphere;
		}

		FORCEINLINE static const bool AreElementsEqual(const FPointBox* A, const FPointBox* B)
		{
			return A->Index == B->Index;
		}

		FORCEINLINE static void ApplyOffset(FPointBox* InNode)
		{
			ensureMsgf(false, TEXT("Not implemented"));
		}

		FORCEINLINE static void SetElementId(const FPointBox* Element, FOctreeElementId2 OctreeElementID)
		{
		}
	};

	class /*PCGEXTENDEDTOOLKIT_API*/ FPointBoxCloud
	{
		using PointBoxCloudOctree = TOctree2<FPointBox*, FPointBoxSemantics>;
		PointBoxCloudOctree* Octree = nullptr;
		TArray<FPointBox*> Boxes;
		FBox CloudBounds;

	public:
		explicit FPointBoxCloud(const UPCGPointData* PointData, const EPCGExPointBoundsSource BoundsSource, const double Epsilon = DBL_EPSILON)
		{
			CloudBounds = PointData->GetBounds();
			Octree = new PointBoxCloudOctree(CloudBounds.GetCenter(), CloudBounds.GetExtent().Length() * 1.5);
			const TArray<FPCGPoint>& Points = PointData->GetPoints();

			CloudBounds = FBox(ForceInit);

			PCGEX_SET_NUM_UNINITIALIZED(Boxes, Points.Num())

			for (int i = 0; i < Points.Num(); ++i)
			{
				FPointBox* NewPointBox = new FPointBox(Points[i], i, BoundsSource, Epsilon);
				CloudBounds += NewPointBox->Box.TransformBy(NewPointBox->Transform);
				Boxes[i] = NewPointBox;
				Octree->AddElement(NewPointBox);
			}
		}

		FORCEINLINE const PointBoxCloudOctree* GetOctree() const { return Octree; }

		~FPointBoxCloud()
		{
			PCGEX_DELETE(Octree)
		}

		bool FindIntersections(FIntersections* InIntersections) const
		{
			const FBoxCenterAndExtent BCAE = InIntersections->GetBoxCenterAndExtent();
			Octree->FindElementsWithBoundsTest(BCAE, [&](const FPointBox* NearbyBox) { NearbyBox->ProcessIntersections(InIntersections); });
			return !InIntersections->Cuts.IsEmpty();
		}

		bool Contains(const FVector& InPosition) const
		{
			if (!CloudBounds.IsInside(InPosition)) { return false; }
			bool bOverlapFound = false;
			Octree->FindNearbyElements(
				InPosition, [&](const FPointBox* NearbyBox)
				{
					if (!bOverlapFound && NearbyBox->Contains(InPosition)) { bOverlapFound = true; }
				});
			return bOverlapFound;
		}

		bool ContainsMinusEpsilon(const FVector& InPosition) const
		{
			if (!CloudBounds.IsInside(InPosition)) { return false; }
			bool bOverlapFound = false;
			Octree->FindNearbyElements(
				InPosition, [&](const FPointBox* NearbyBox)
				{
					if (!bOverlapFound && NearbyBox->ContainsMinusEpsilon(InPosition)) { bOverlapFound = true; }
				});
			return bOverlapFound;
		}

		bool Contains(const FVector& InPosition, TArray<FPointBox*>& OutOverlaps) const
		{
			if (!CloudBounds.IsInside(InPosition)) { return false; }
			Octree->FindNearbyElements(
				InPosition, [&](const FPointBox* NearbyBox)
				{
					if (NearbyBox->Contains(InPosition)) { OutOverlaps.Add(*(Boxes.GetData() + NearbyBox->Index)); }
				});
			return !OutOverlaps.IsEmpty();
		}

		bool ContainsMinusEpsilon(const FVector& InPosition, TArray<FPointBox*>& OutOverlaps) const
		{
			if (!CloudBounds.IsInside(InPosition)) { return false; }
			Octree->FindNearbyElements(
				InPosition, [&](const FPointBox* NearbyBox)
				{
					if (NearbyBox->ContainsMinusEpsilon(InPosition)) { OutOverlaps.Add(*(Boxes.GetData() + NearbyBox->Index)); }
				});
			return !OutOverlaps.IsEmpty();
		}

		bool LooseOverlaps(const UPCGPointData* InPointData) const
		{
			const FBox PointBounds = InPointData->GetBounds();
			if (CloudBounds.Intersect(PointBounds)) { return true; }
			return CloudBounds.IsInside(PointBounds);
		}

		bool Overlaps(const UPCGPointData* InPointData) const
		{
			const FBox PointBounds = InPointData->GetBounds();
			if (CloudBounds.Intersect(PointBounds)) { return true; }
			return CloudBounds.IsInside(PointBounds);
		}

		bool Overlaps(const TArrayView<PCGExGraph::FIndexedEdge>& Edges, const UPCGPointData* InPointData) const
		{
			const FBox PointBounds = InPointData->GetBounds();

			if (!CloudBounds.Intersect(PointBounds))
			{
				if (!CloudBounds.IsInside(PointBounds))
				{
					// No intersection nor encapsulation, don't bother
					return false;
				}
			}

			return false;
		}

		bool Encompass(const UPCGPointData* InPointData) const
		{
			const FBox PointBounds = InPointData->GetBounds();

			if (!CloudBounds.Intersect(PointBounds))
			{
				if (!CloudBounds.IsInside(PointBounds))
				{
					// No intersection nor encapsulation, don't bother
					return false;
				}
			}

			const TArray<FPCGPoint>& Points = InPointData->GetPoints();

			return false;
		}

		bool Sample(const FPCGPoint& Point, const EPCGExPointBoundsSource BoundsSource, TArray<FSample>& OutSample) const
		{
			const FBoxCenterAndExtent BCAE = FBoxCenterAndExtent(Point.Transform.GetLocation(), PCGExMath::GetLocalBounds(Point, BoundsSource).GetExtent());
			Octree->FindElementsWithBoundsTest(
				BCAE, [&](const FPointBox* NearbyBox)
				{
					FSample& Sample = OutSample.Emplace_GetRef();
					NearbyBox->Sample(Point, Sample);
				});

			return !OutSample.IsEmpty();
		}
	};
}
