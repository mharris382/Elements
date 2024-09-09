// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "ElementsSubsystem.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FGameplayTag;
#ifdef ELEMENTS_UE_ElementsSubsystem_generated_h
#error "ElementsSubsystem.generated.h already included, missing '#pragma once' in ElementsSubsystem.h"
#endif
#define ELEMENTS_UE_ElementsSubsystem_generated_h

#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_15_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FElementData_Statics; \
	ELEMENTS_UE_API static class UScriptStruct* StaticStruct(); \
	typedef FTableRowBase Super;


template<> ELEMENTS_UE_API UScriptStruct* StaticStruct<struct FElementData>();

#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execGetElementRelationship);


#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUElementsSubsystem(); \
	friend struct Z_Construct_UClass_UElementsSubsystem_Statics; \
public: \
	DECLARE_CLASS(UElementsSubsystem, UGameInstanceSubsystem, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/Elements_UE"), NO_API) \
	DECLARE_SERIALIZER(UElementsSubsystem)


#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UElementsSubsystem(); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UElementsSubsystem(UElementsSubsystem&&); \
	UElementsSubsystem(const UElementsSubsystem&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UElementsSubsystem); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UElementsSubsystem); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UElementsSubsystem) \
	NO_API virtual ~UElementsSubsystem();


#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_51_PROLOG
#define FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_INCLASS_NO_PURE_DECLS \
	FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h_54_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ELEMENTS_UE_API UClass* StaticClass<class UElementsSubsystem>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Elements_UE_Source_Elements_UE_ElementsSubsystem_h


#define FOREACH_ENUM_EELEMENTRELATIONSHIP(op) \
	op(Neutral) \
	op(Strong) \
	op(Weak) 

enum EElementRelationship : uint8;
template<> ELEMENTS_UE_API UEnum* StaticEnum<EElementRelationship>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
