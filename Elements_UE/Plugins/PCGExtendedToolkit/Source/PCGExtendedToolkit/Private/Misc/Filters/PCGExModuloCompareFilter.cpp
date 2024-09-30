﻿// Copyright Timothé Lapetite 2024
// Released under the MIT license https://opensource.org/license/MIT/

#include "Misc/Filters/PCGExModuloCompareFilter.h"

#define LOCTEXT_NAMESPACE "PCGExCompareFilterDefinition"
#define PCGEX_NAMESPACE CompareFilterDefinition

PCGExPointFilter::TFilter* UPCGExModuloCompareFilterFactory::CreateFilter() const
{
	return new PCGExPointsFilter::TModuloComparisonFilter(this);
}

bool PCGExPointsFilter::TModuloComparisonFilter::Init(const FPCGContext* InContext, PCGExData::FFacade* InPointDataFacade)
{
	if (!TFilter::Init(InContext, InPointDataFacade)) { return false; }

	OperandA = PointDataFacade->GetScopedBroadcaster<double>(TypedFilterFactory->Config.OperandA);

	if (!OperandA)
	{
		PCGE_LOG_C(Error, GraphAndLog, InContext, FText::Format(FTEXT("Invalid Operand A attribute: \"{0}\"."), FText::FromName(TypedFilterFactory->Config.OperandA.GetName())));
		return false;
	}

	if (TypedFilterFactory->Config.OperandBSource == EPCGExFetchType::Attribute)
	{
		OperandB = PointDataFacade->GetScopedBroadcaster<double>(TypedFilterFactory->Config.OperandB);

		if (!OperandB)
		{
			PCGE_LOG_C(Error, GraphAndLog, InContext, FText::Format(FTEXT("Invalid Operand B attribute: \"{0}\"."), FText::FromName(TypedFilterFactory->Config.OperandB.GetName())));
			return false;
		}
	}

	if (TypedFilterFactory->Config.CompareAgainst == EPCGExFetchType::Attribute)
	{
		OperandC = PointDataFacade->GetScopedBroadcaster<double>(TypedFilterFactory->Config.OperandC);

		if (!OperandC)
		{
			PCGE_LOG_C(Error, GraphAndLog, InContext, FText::Format(FTEXT("Invalid Operand C attribute: \"{0}\"."), FText::FromName(TypedFilterFactory->Config.OperandB.GetName())));
			return false;
		}
	}

	return true;
}

PCGEX_CREATE_FILTER_FACTORY(ModuloCompare)

#if WITH_EDITOR
FString UPCGExModuloCompareFilterProviderSettings::GetDisplayName() const
{
	FString DisplayName = Config.OperandA.GetName().ToString() + " % ";

	if (Config.OperandBSource == EPCGExFetchType::Attribute) { DisplayName += Config.OperandB.GetName().ToString(); }
	else { DisplayName += FString::Printf(TEXT("%.3f "), (static_cast<int32>(1000 * Config.OperandBConstant) / 1000.0)); }

	DisplayName += PCGExCompare::ToString(Config.Comparison);

	if (Config.CompareAgainst == EPCGExFetchType::Attribute) { DisplayName += Config.OperandC.GetName().ToString(); }
	else { DisplayName += FString::Printf(TEXT(" %.3f"), (static_cast<int32>(1000 * Config.OperandCConstant) / 1000.0)); }

	return DisplayName;
}
#endif

#undef LOCTEXT_NAMESPACE
#undef PCGEX_NAMESPACE
