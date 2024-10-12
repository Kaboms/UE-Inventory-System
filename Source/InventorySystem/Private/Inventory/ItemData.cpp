#include "Inventory/ItemData.h"
#include "Inventory/ItemBase.h"

UItemBase* UItemData::ConstructItemInstance(UObject* InOuter, UItemData* ItemData)
{
    return ItemData->ConstructItemInstance(InOuter);
}

UItemData::UItemData(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    ItemInstanceClass = UItemBase::StaticClass();
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
    if (MetadataEntriesCache.IsEmpty())
    {
        for (const auto& MetadataEntry : MetadataEntries)
        {
            MetadataEntriesCache.Add(MetadataEntry->GetClass(), MetadataEntry);
        }
    }

    if (UItemMetadata** ItemMetadata = MetadataEntriesCache.Find(MetadataClass))
    {
        return *ItemMetadata;
    }

    return nullptr;
}
