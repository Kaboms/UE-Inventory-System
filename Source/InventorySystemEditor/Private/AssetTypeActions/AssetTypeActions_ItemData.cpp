#include "AssetTypeActions_ItemData.h"
#include "Inventory/ItemData.h"
#include "ClassIconFinder.h"
#include "Styling/SlateIconFinder.h"

UClass* FAssetTypeActions_ItemData::GetSupportedClass() const
{
    return UItemData::StaticClass();;
}

const FSlateBrush* FAssetTypeActions_ItemData::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const
{
	///InventorySystem/Icons/T_Item_Dummy.T_Item_Dummy

	FSlateBrush* Icon = nullptr;
	if (UItemData* ItemData = Cast<UItemData>(InAssetData.GetAsset()))
	{
		if (IsValid(ItemData->Icon))
		{
			Icon = new FSlateBrush();
			Icon->SetResourceObject(ItemData->Icon);
			Icon->ImageSize = FVector2D(ItemData->Icon->GetSizeX(), ItemData->Icon->GetSizeY());
		}
	}

	return Icon ? Icon : FAppStyle::GetNoBrush();
}