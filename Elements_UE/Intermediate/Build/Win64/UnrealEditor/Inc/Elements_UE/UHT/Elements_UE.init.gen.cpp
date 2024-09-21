// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeElements_UE_init() {}
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_AttackActionCompleted__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_AttackActionStartedDelegate__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_CharacterBaseHitReactDelegate__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_CharacterDiedDelegate__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_ElementChangedDelegate__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_OnAttackFinished__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_OnAttributeChanged__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_OnCooldownChanged__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_OnGameplayEffectStackChanged__DelegateSignature();
	ELEMENTS_UE_API UFunction* Z_Construct_UDelegateFunction_Elements_UE_ReceivedDamageDelegate__DelegateSignature();
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Elements_UE;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Elements_UE()
	{
		if (!Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_AttackActionCompleted__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_AttackActionStartedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_CharacterBaseHitReactDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_CharacterDiedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_ElementChangedDelegate__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_OnAttackFinished__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_OnAttributeChanged__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_OnCooldownChanged__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_OnGameplayEffectStackChanged__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_Elements_UE_ReceivedDamageDelegate__DelegateSignature,
			};
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Elements_UE",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x1E2CEE09,
				0xD8238A07,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Elements_UE.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Elements_UE(Z_Construct_UPackage__Script_Elements_UE, TEXT("/Script/Elements_UE"), Z_Registration_Info_UPackage__Script_Elements_UE, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x1E2CEE09, 0xD8238A07));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
