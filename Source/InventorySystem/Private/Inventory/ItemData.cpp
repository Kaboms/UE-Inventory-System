// (c) 2021 Sonic Terror

#include "Inventory/ItemData.h"
#include "Inventory/ItemBase.h"

UItemData::UItemData()
{
    ItemInstanceClass = UItemBase::StaticClass();
}
