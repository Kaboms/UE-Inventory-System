#include "Equipment/EquipSlotBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemData.h"
#include "Equipment/EquipmentContainer.h"
#include "Inventory/ItemBase.h"

bool UEquipSlotBase::Equip(UObject* Instigator, UContainerItemBase* ContainerItemToEquip)
{
    if (!CanEquip(ContainerItemToEquip->GetItemData()))
        return false;

    if (IsValid(ContainerItem))
    {
        if (ContainerItem->IsItemSameType(ContainerItemToEquip))
        {
            return ContainerItem->MergeWithOther(ContainerItemToEquip);
            // We don't need call Execute_Equip if an item is already equiped
        }
        else
        {
            if (ContainerItem->GetItem()->Implements<UEquipableInterface>())
            {
                IEquipableInterface::Execute_TakeOff(ContainerItem->GetItem());
            }

            ContainerItemToEquip->Swap(ContainerItem);
        }
    }
    else
    {
        ContainerItem = ContainerItemToEquip;
        ContainerItem->Container = EquipmentContainer;
    }

    if (ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_Equip(ContainerItem->GetItem(), Instigator);
    }

    OnEquiped.Broadcast();

    return true;
}

bool UEquipSlotBase::CanEquip(UItemData* ItemData)
{
    if (ItemData->Type.HasTag(RequiredItemType))
    {
        if (IsValid(ContainerItem))
        {
            return ContainerItem->GetItemData() == ItemData && ContainerItem->CanAddItem();
        }
        return true;
    }
    return false;
}

bool UEquipSlotBase::HasItem()
{
    return IsValid(ContainerItem);
}

bool UEquipSlotBase::CanEquipContainerItem(UContainerItemBase* ContainerItemToEquip)
{
    if (IsValid(ContainerItemToEquip))
    {
        return CanEquip(ContainerItemToEquip->GetItemData());
    }
    return false;
}

void UEquipSlotBase::TakeOff()
{
    if (IsValid(ContainerItem) && ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_TakeOff(ContainerItem->GetItem());
    }

    ContainerItem = nullptr;

    OnTakeOff.Broadcast();
}

void UEquipSlotBase::Select()
{
    if (IsValid(ContainerItem) && ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_Select(ContainerItem->GetItem());
    }
}
