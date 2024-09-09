// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Elements_UE/ElementsSubsystem.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeElementsSubsystem() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
ELEMENTS_UE_API UClass* Z_Construct_UClass_UElementsSubsystem();
ELEMENTS_UE_API UClass* Z_Construct_UClass_UElementsSubsystem_NoRegister();
ELEMENTS_UE_API UEnum* Z_Construct_UEnum_Elements_UE_EElementRelationship();
ELEMENTS_UE_API UScriptStruct* Z_Construct_UScriptStruct_FElementData();
ENGINE_API UClass* Z_Construct_UClass_UDataTable_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FTableRowBase();
GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTag();
GAMEPLAYTAGS_API UScriptStruct* Z_Construct_UScriptStruct_FGameplayTagContainer();
UPackage* Z_Construct_UPackage__Script_Elements_UE();
// End Cross Module References

// Begin ScriptStruct FElementData
static_assert(std::is_polymorphic<FElementData>() == std::is_polymorphic<FTableRowBase>(), "USTRUCT FElementData cannot be polymorphic unless super FTableRowBase is polymorphic");
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_ElementData;
class UScriptStruct* FElementData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_ElementData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_ElementData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FElementData, (UObject*)Z_Construct_UPackage__Script_Elements_UE(), TEXT("ElementData"));
	}
	return Z_Registration_Info_UScriptStruct_ElementData.OuterSingleton;
}
template<> ELEMENTS_UE_API UScriptStruct* StaticStruct<FElementData>()
{
	return FElementData::StaticStruct();
}
struct Z_Construct_UScriptStruct_FElementData_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DisplayName_MetaData[] = {
		{ "Category", "Element" },
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ElementTag_MetaData[] = {
		{ "Category", "Element" },
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ElementColorID_MetaData[] = {
		{ "Category", "Element" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//color to represent the element, used for UI and to drive visual effects \n" },
#endif
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "color to represent the element, used for UI and to drive visual effects" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_StrongAgainst_MetaData[] = {
		{ "Category", "Element" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//comma separated list of elements that this element is strong against\n" },
#endif
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "comma separated list of elements that this element is strong against" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_DisplayName;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ElementTag;
	static const UECodeGen_Private::FStructPropertyParams NewProp_ElementColorID;
	static const UECodeGen_Private::FStructPropertyParams NewProp_StrongAgainst;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FElementData>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UScriptStruct_FElementData_Statics::NewProp_DisplayName = { "DisplayName", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FElementData, DisplayName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DisplayName_MetaData), NewProp_DisplayName_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FElementData_Statics::NewProp_ElementTag = { "ElementTag", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FElementData, ElementTag), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ElementTag_MetaData), NewProp_ElementTag_MetaData) }; // 1298103297
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FElementData_Statics::NewProp_ElementColorID = { "ElementColorID", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FElementData, ElementColorID), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ElementColorID_MetaData), NewProp_ElementColorID_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FElementData_Statics::NewProp_StrongAgainst = { "StrongAgainst", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FElementData, StrongAgainst), Z_Construct_UScriptStruct_FGameplayTagContainer, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_StrongAgainst_MetaData), NewProp_StrongAgainst_MetaData) }; // 3352185621
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FElementData_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FElementData_Statics::NewProp_DisplayName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FElementData_Statics::NewProp_ElementTag,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FElementData_Statics::NewProp_ElementColorID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FElementData_Statics::NewProp_StrongAgainst,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FElementData_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FElementData_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_Elements_UE,
	Z_Construct_UScriptStruct_FTableRowBase,
	&NewStructOps,
	"ElementData",
	Z_Construct_UScriptStruct_FElementData_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FElementData_Statics::PropPointers),
	sizeof(FElementData),
	alignof(FElementData),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FElementData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FElementData_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FElementData()
{
	if (!Z_Registration_Info_UScriptStruct_ElementData.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_ElementData.InnerSingleton, Z_Construct_UScriptStruct_FElementData_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_ElementData.InnerSingleton;
}
// End ScriptStruct FElementData

// Begin Enum EElementRelationship
static FEnumRegistrationInfo Z_Registration_Info_UEnum_EElementRelationship;
static UEnum* EElementRelationship_StaticEnum()
{
	if (!Z_Registration_Info_UEnum_EElementRelationship.OuterSingleton)
	{
		Z_Registration_Info_UEnum_EElementRelationship.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_Elements_UE_EElementRelationship, (UObject*)Z_Construct_UPackage__Script_Elements_UE(), TEXT("EElementRelationship"));
	}
	return Z_Registration_Info_UEnum_EElementRelationship.OuterSingleton;
}
template<> ELEMENTS_UE_API UEnum* StaticEnum<EElementRelationship>()
{
	return EElementRelationship_StaticEnum();
}
struct Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//struct FElementRelationships\n//{\n//\x09TMap<FGameplayTagContainer, EElementRelationship> Relationships;\n//};\n" },
#endif
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
		{ "Neutral.Name", "Neutral" },
		{ "Strong.Name", "Strong" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "struct FElementRelationships\n{\n       TMap<FGameplayTagContainer, EElementRelationship> Relationships;\n};" },
#endif
		{ "Weak.Name", "Weak" },
	};
#endif // WITH_METADATA
	static constexpr UECodeGen_Private::FEnumeratorParam Enumerators[] = {
		{ "Neutral", (int64)Neutral },
		{ "Strong", (int64)Strong },
		{ "Weak", (int64)Weak },
	};
	static const UECodeGen_Private::FEnumParams EnumParams;
};
const UECodeGen_Private::FEnumParams Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::EnumParams = {
	(UObject*(*)())Z_Construct_UPackage__Script_Elements_UE,
	nullptr,
	"EElementRelationship",
	"EElementRelationship",
	Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::Enumerators,
	RF_Public|RF_Transient|RF_MarkAsNative,
	UE_ARRAY_COUNT(Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::Enumerators),
	EEnumFlags::None,
	(uint8)UEnum::ECppForm::Regular,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::Enum_MetaDataParams), Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::Enum_MetaDataParams)
};
UEnum* Z_Construct_UEnum_Elements_UE_EElementRelationship()
{
	if (!Z_Registration_Info_UEnum_EElementRelationship.InnerSingleton)
	{
		UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EElementRelationship.InnerSingleton, Z_Construct_UEnum_Elements_UE_EElementRelationship_Statics::EnumParams);
	}
	return Z_Registration_Info_UEnum_EElementRelationship.InnerSingleton;
}
// End Enum EElementRelationship

// Begin Class UElementsSubsystem Function GetElementRelationship
struct Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics
{
	struct ElementsSubsystem_eventGetElementRelationship_Parms
	{
		FGameplayTag Attacker;
		FGameplayTag Defender;
		TEnumAsByte<EElementRelationship> ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Element" },
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Attacker;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Defender;
	static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_Attacker = { "Attacker", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ElementsSubsystem_eventGetElementRelationship_Parms, Attacker), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(0, nullptr) }; // 1298103297
const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_Defender = { "Defender", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ElementsSubsystem_eventGetElementRelationship_Parms, Defender), Z_Construct_UScriptStruct_FGameplayTag, METADATA_PARAMS(0, nullptr) }; // 1298103297
const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ElementsSubsystem_eventGetElementRelationship_Parms, ReturnValue), Z_Construct_UEnum_Elements_UE_EElementRelationship, METADATA_PARAMS(0, nullptr) }; // 4116450939
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_Attacker,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_Defender,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UElementsSubsystem, nullptr, "GetElementRelationship", nullptr, nullptr, Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::PropPointers), sizeof(Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::ElementsSubsystem_eventGetElementRelationship_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::Function_MetaDataParams), Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::ElementsSubsystem_eventGetElementRelationship_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UElementsSubsystem::execGetElementRelationship)
{
	P_GET_STRUCT(FGameplayTag,Z_Param_Attacker);
	P_GET_STRUCT(FGameplayTag,Z_Param_Defender);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(TEnumAsByte<EElementRelationship>*)Z_Param__Result=P_THIS->GetElementRelationship(Z_Param_Attacker,Z_Param_Defender);
	P_NATIVE_END;
}
// End Class UElementsSubsystem Function GetElementRelationship

// Begin Class UElementsSubsystem
void UElementsSubsystem::StaticRegisterNativesUElementsSubsystem()
{
	UClass* Class = UElementsSubsystem::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetElementRelationship", &UElementsSubsystem::execGetElementRelationship },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UElementsSubsystem);
UClass* Z_Construct_UClass_UElementsSubsystem_NoRegister()
{
	return UElementsSubsystem::StaticClass();
}
struct Z_Construct_UClass_UElementsSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "ElementsSubsystem.h" },
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ElementDataTable_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Store the loaded element data\n" },
#endif
		{ "ModuleRelativePath", "ElementsSubsystem.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Store the loaded element data" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ElementDataTable;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UElementsSubsystem_GetElementRelationship, "GetElementRelationship" }, // 2487332325
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UElementsSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UElementsSubsystem_Statics::NewProp_ElementDataTable = { "ElementDataTable", nullptr, (EPropertyFlags)0x0144000000000000, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UElementsSubsystem, ElementDataTable), Z_Construct_UClass_UDataTable_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ElementDataTable_MetaData), NewProp_ElementDataTable_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UElementsSubsystem_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UElementsSubsystem_Statics::NewProp_ElementDataTable,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UElementsSubsystem_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UElementsSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_Elements_UE,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UElementsSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UElementsSubsystem_Statics::ClassParams = {
	&UElementsSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_UElementsSubsystem_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_UElementsSubsystem_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UElementsSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UElementsSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UElementsSubsystem()
{
	if (!Z_Registration_Info_UClass_UElementsSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UElementsSubsystem.OuterSingleton, Z_Construct_UClass_UElementsSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UElementsSubsystem.OuterSingleton;
}
template<> ELEMENTS_UE_API UClass* StaticClass<UElementsSubsystem>()
{
	return UElementsSubsystem::StaticClass();
}
UElementsSubsystem::UElementsSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UElementsSubsystem);
UElementsSubsystem::~UElementsSubsystem() {}
// End Class UElementsSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics
{
	static constexpr FEnumRegisterCompiledInInfo EnumInfo[] = {
		{ EElementRelationship_StaticEnum, TEXT("EElementRelationship"), &Z_Registration_Info_UEnum_EElementRelationship, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 4116450939U) },
	};
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FElementData::StaticStruct, Z_Construct_UScriptStruct_FElementData_Statics::NewStructOps, TEXT("ElementData"), &Z_Registration_Info_UScriptStruct_ElementData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FElementData), 2633413066U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UElementsSubsystem, UElementsSubsystem::StaticClass, TEXT("UElementsSubsystem"), &Z_Registration_Info_UClass_UElementsSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UElementsSubsystem), 3895627502U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_1417378400(TEXT("/Script/Elements_UE"),
	Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::ScriptStructInfo),
	Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_Statics::EnumInfo));
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
