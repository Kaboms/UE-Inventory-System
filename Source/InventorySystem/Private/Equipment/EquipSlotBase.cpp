#include "Equipment/EquipSlotBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemData.h"
#include "Equipment/EquipmentContainer.h"
#include "Inventory/ItemBase.h"

void UEquipSlotBase::Equip(UObject* Instigator, UContainerItemBase* ContainerItemToEquip)
{
    if (IsValid(ContainerItem) && ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_TakeOff(ContainerItem->GetItem());
    }

    ContainerItem = ContainerItemToEquip;
    ContainerItem->Container = EquipmentContainer;

    if (ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_Equip(ContainerItem->GetItem(), Instigator);
    }

    OnEquiped.Broadcast();
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
    if (IsValid(ContainerItem) && ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_TakeOff(ContainerItem->GetItem());

        ContainerItem = nullptr;
    }

    OnTakeOff.Broadcast();
}

void UEquipSlotBase::Select()
{
    if (IsValid(ContainerItem) && ContainerItem->GetItem()->Implements<UEquipableInterface>())
    {
        IEquipableInterface::Execute_Select(ContainerItem->GetItem());
    }
}
