// (c) 2021 Sonic Terror

#include "Inventory/ItemData.h"
#include "Inventory/ItemBase.h"

UItemBase* UItemData::ConstructItemInstance(UObject* InOuter, UItemData* ItemData)
{
    return ItemData->ConstructItemInstance(InOuter);
}

UItemData::UItemData()
{
    ItemInstanceClass = UItemBase::StaticClass();
}

UItemBase* UItemData::ConstructItemInstance(UObject* Outer)
{
    UItemBase* ItemBase = NewObject<UItemBase>(Outer, ItemInstanceClass);
    ItemBase->ItemData = this;

    return ItemBase;
}
