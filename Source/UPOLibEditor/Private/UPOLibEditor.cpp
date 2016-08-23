// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UPOLibEditorPrivatePCH.h"

#include "ShapeHelperComponentVis.h"

DEFINE_LOG_CATEGORY(UPOLibEditor);

#define LOCTEXT_NAMESPACE "FUPOLibEditorModule"

void FUPOLibEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FShapeHelperComponentVis::Reg();
}

void FUPOLibEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FShapeHelperComponentVis::Unreg();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUPOLibEditorModule, UPOLibEditor)