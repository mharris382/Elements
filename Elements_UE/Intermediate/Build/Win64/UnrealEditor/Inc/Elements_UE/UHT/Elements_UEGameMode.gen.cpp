// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Elements_UE/Elements_UEGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeElements_UEGameMode() {}

// Begin Cross Module References
ELEMENTS_UE_API UClass* Z_Construct_UClass_AElements_UEGameMode();
ELEMENTS_UE_API UClass* Z_Construct_UClass_AElements_UEGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_Elements_UE();
// End Cross Module References

// Begin Class AElements_UEGameMode
void AElements_UEGameMode::StaticRegisterNativesAElements_UEGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AElements_UEGameMode);
UClass* Z_Construct_UClass_AElements_UEGameMode_NoRegister()
{
	return AElements_UEGameMode::StaticClass();
}
struct Z_Construct_UClass_AElements_UEGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "Elements_UEGameMode.h" },
		{ "ModuleRelativePath", "Elements_UEGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AElements_UEGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AElements_UEGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_Elements_UE,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AElements_UEGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AElements_UEGameMode_Statics::ClassParams = {
	&AElements_UEGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AElements_UEGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AElements_UEGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AElements_UEGameMode()
{
	if (!Z_Registration_Info_UClass_AElements_UEGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AElements_UEGameMode.OuterSingleton, Z_Construct_UClass_AElements_UEGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AElements_UEGameMode.OuterSingleton;
}
template<> ELEMENTS_UE_API UClass* StaticClass<AElements_UEGameMode>()
{
	return AElements_UEGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AElements_UEGameMode);
AElements_UEGameMode::~AElements_UEGameMode() {}
// End Class AElements_UEGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UEGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AElements_UEGameMode, AElements_UEGameMode::StaticClass, TEXT("AElements_UEGameMode"), &Z_Registration_Info_UClass_AElements_UEGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AElements_UEGameMode), 3004765851U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UEGameMode_h_1095535880(TEXT("/Script/Elements_UE"),
	Z_CompiledInDeferFile_FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UEGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UEGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
