#include "Equipment/EquipSlotBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemData.h"

void UEquipSlotBase::Equip(UContainerItemBase* ContainerItemToEquip)
{
    if (IsValid(ContainerItemToEquip))
    {
        if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItemToEquip->GetItem()))
        {
            EquipableItem->TakeOff();
        }
    }

    ContainerItem = ContainerItemToEquip;

    if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItem->GetItem()))
    {
        EquipableItem->Equip();
    }
}

bool UEquipSlotBase::CanEquip(UItemData* ItemData)
{
    return ItemData->Type.HasTag(RequiredItemType);
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
    if (IsValid(ContainerItem))
    {
        if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItem->GetItem()))
        {
            EquipableItem->TakeOff();
        }

        ContainerItem = nullptr;
    }
}

void UEquipSlotBase::Select()
{
    if (IsValid(ContainerItem))
    {
        if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItem->GetItem()))
        {
            EquipableItem->Select();
        }
    }
}
