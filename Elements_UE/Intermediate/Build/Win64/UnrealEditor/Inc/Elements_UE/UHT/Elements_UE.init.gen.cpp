// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeElements_UE_init() {}
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_OnAttackFinished__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Elements_UE;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Elements_UE()
	{
		if (!Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_OnAttackFinished__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Elements_UE",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x1F216E79,
				0xC8E726F7,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Elements_UE(Z_Construct_UPackage__Script_Elements_UE, TEXT("/Script/Elements_UE"), Z_Registration_Info_UPackage__Script_Elements_UE, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x1F216E79, 0xC8E726F7));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
