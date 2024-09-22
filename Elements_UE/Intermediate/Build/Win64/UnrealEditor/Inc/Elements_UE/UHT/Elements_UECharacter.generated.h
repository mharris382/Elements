// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Elements_UECharacter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ELEMENTS_UE_Elements_UECharacter_generated_h
#error "Elements_UECharacter.generated.h already included, missing '#pragma once' in Elements_UECharacter.h"
#endif
#define ELEMENTS_UE_Elements_UECharacter_generated_h

#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_16_DELEGATE \
ELEMENTS_UE_API void FOnAttackFinished_DelegateWrapper(const FMulticastScriptDelegate& OnAttackFinished);


#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execNotifyAttackFinished);


#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAElements_UECharacter(); \
	friend struct Z_Construct_UClass_AElements_UECharacter_Statics; \
public: \
	DECLARE_CLASS(AElements_UECharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Elements_UE"), NO_API) \
	DECLARE_SERIALIZER(AElements_UECharacter)


#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AElements_UECharacter(AElements_UECharacter&&); \
	AElements_UECharacter(const AElements_UECharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AElements_UECharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AElements_UECharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AElements_UECharacter) \
	NO_API virtual ~AElements_UECharacter();


#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_22_PROLOG
#define FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_INCLASS_NO_PURE_DECLS \
	FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h_25_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ELEMENTS_UE_API UClass* StaticClass<class AElements_UECharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Admin_Documents_UE_Projects_Elements_Elements_UE_Source_Elements_UE_Elements_UECharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
