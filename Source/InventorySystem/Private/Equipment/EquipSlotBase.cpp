// (c) 2021 Sonic Terror

#include "Equipment/EquipSlotBase.h"
#include "Inventory/ItemBase.h"

void UEquipSlotBase::Equip(UItemBase* ItemToEquip)
{
    if (IsValid(Item))
    {
        if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(Item))
        {
            EquipableItem->TakeOff();
        }
    }

    Item = ItemToEquip;

    if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(Item))
    {
        EquipableItem->Equip();
    }
}
