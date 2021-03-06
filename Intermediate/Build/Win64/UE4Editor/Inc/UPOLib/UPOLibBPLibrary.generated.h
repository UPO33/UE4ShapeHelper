// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UShapeComponent;
struct FVector;
#ifdef UPOLIB_UPOLibBPLibrary_generated_h
#error "UPOLibBPLibrary.generated.h already included, missing '#pragma once' in UPOLibBPLibrary.h"
#endif
#define UPOLIB_UPOLibBPLibrary_generated_h

#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execIsPoinInShapeComponent) \
	{ \
		P_GET_OBJECT(UShapeComponent,Z_Param_shape); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_pointWS); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UUPOLibBPLibrary::IsPoinInShapeComponent(Z_Param_shape,Z_Param_Out_pointWS); \
		P_NATIVE_END; \
	}


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execIsPoinInShapeComponent) \
	{ \
		P_GET_OBJECT(UShapeComponent,Z_Param_shape); \
		P_GET_STRUCT_REF(FVector,Z_Param_Out_pointWS); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(bool*)Z_Param__Result=UUPOLibBPLibrary::IsPoinInShapeComponent(Z_Param_shape,Z_Param_Out_pointWS); \
		P_NATIVE_END; \
	}


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUUPOLibBPLibrary(); \
	friend UPOLIB_API class UClass* Z_Construct_UClass_UUPOLibBPLibrary(); \
	public: \
	DECLARE_CLASS(UUPOLibBPLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UPOLib"), NO_API) \
	DECLARE_SERIALIZER(UUPOLibBPLibrary) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_INCLASS \
	private: \
	static void StaticRegisterNativesUUPOLibBPLibrary(); \
	friend UPOLIB_API class UClass* Z_Construct_UClass_UUPOLibBPLibrary(); \
	public: \
	DECLARE_CLASS(UUPOLibBPLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/UPOLib"), NO_API) \
	DECLARE_SERIALIZER(UUPOLibBPLibrary) \
	/** Indicates whether the class is compiled into the engine */ \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUPOLibBPLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUPOLibBPLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUPOLibBPLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUPOLibBPLibrary); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UUPOLibBPLibrary(const UUPOLibBPLibrary& InCopy); \
public:


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUPOLibBPLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UUPOLibBPLibrary(const UUPOLibBPLibrary& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUPOLibBPLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUPOLibBPLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUPOLibBPLibrary)


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_25_PROLOG
#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_RPC_WRAPPERS \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_INCLASS \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_RPC_WRAPPERS_NO_PURE_DECLS \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_INCLASS_NO_PURE_DECLS \
	Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h_28_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Neghab_Plugins_UPOLib_Source_UPOLib_Public_UPOLibBPLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
