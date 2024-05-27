// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemEditor.h"
#include "AssetTypeActions/AssetTypeActions_ItemData.h"
#include "InventorySystemEditorStyle.h"

#define LOCTEXT_NAMESPACE "FInventorySystemEditorModule"

void FInventorySystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_EDITOR
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type InventoryCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Inventory")), LOCTEXT("InventorySystemAssetCategory", "Inventory"));

	AssetTools.RegisterAssetTypeActions(MakeShared<FAssetTypeActions_ItemData>(InventoryCategoryBit));
#endif

	FInventorySystemEditorStyle::Register();
}

void FInventorySystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FInventorySystemEditorStyle::Unregister();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventorySystemEditorModule, InventorySystemEditor)