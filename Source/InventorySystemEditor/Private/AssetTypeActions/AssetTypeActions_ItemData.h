#pragma once

#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"
#include "Containers/Array.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Text.h"
#include "Math/Color.h"
#include "Templates/SharedPointer.h"

class UClass;
struct FAssetData;

class FAssetTypeActions_ItemData : public FAssetTypeActions_Base
{
public:
	//~ Begin IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ItemData", "Item Data"); }
	virtual FColor GetTypeColor() const override { return FColor(155, 0, 255); }
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Blueprint; }
	virtual UClass* GetSupportedClass() const override;
	virtual bool CanFilter() override { return true; }
	virtual bool IsImportedAsset() const override { return false; }

	virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
	//~ End IAssetTypeActions Implementation
};
