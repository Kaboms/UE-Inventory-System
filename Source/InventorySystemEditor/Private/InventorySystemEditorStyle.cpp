// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemEditorStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"


FInventorySystemEditorStyle::FInventorySystemEditorStyle() :
    FSlateStyleSet("InventorySystemEditorStyle")
{
	static const FVector2D IconSize16x16(16.0f, 16.0f);
	static const FVector2D IconSize32x32(16.0f, 16.0f);
	static const FVector2D IconSize64x64(64.0f, 64.0f);

	static const FSlateColor DefaultForeground(FLinearColor(0.72f, 0.72f, 0.72f, 1.f));

	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("InventorySystem");
	check(Plugin.IsValid());

	SetContentRoot(Plugin->GetContentDir() / TEXT("Slate"));

	Set("ClassIcon.ItemData", new IMAGE_BRUSH(TEXT("Icons/ItemDataAssetIcon"), IconSize16x16));
	Set("ClassThumbnail.ItemData", new IMAGE_BRUSH(TEXT("Icons/ItemDataAssetIcon_64"), IconSize64x64));
}

void FInventorySystemEditorStyle::Register()
{
	FSlateStyleRegistry::RegisterSlateStyle(Get());
}

void FInventorySystemEditorStyle::Unregister()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(Get());
}

FInventorySystemEditorStyle& FInventorySystemEditorStyle::Get()
{
	static FInventorySystemEditorStyle Instance;
	return Instance;
}
