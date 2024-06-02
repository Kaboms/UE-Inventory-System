#include "Inventory/ItemData.h"
#include "Inventory/ItemBase.h"

UItemBase* UItemData::ConstructItemInstance(UObject* InOuter, UItemData* ItemData)
{
    return ItemData->ConstructItemInstance(InOuter);
}

UItemData::UItemData()
{
    ItemInstanceClass = UItemBase::StaticClass();

    for (const auto& MetadataEntry : MetadataEntries)
    {
        MetadataEntriesCache.Add(MetadataEntry->GetClass(), MetadataEntry);
    }
}

UItemBase* UItemData::ConstructItemInstance(UObject* Outer)
{
    UItemBase* ItemBase = NewObject<UItemBase>(Outer, ItemInstanceClass);
    ItemBase->ItemData = this;
    ItemBase->Init();

    return ItemBase;
}

UItemMetadata* UItemData::FindMetadataByClass(TSubclassOf<UItemMetadata> MetadataClass)
{
    if (UItemMetadata** ItemMetadata = MetadataEntriesCache.Find(MetadataClass))
    {
        return *ItemMetadata;
    }

    return nullptr;
}
