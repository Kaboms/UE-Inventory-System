#include "Equipment/EquipSlotBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemData.h"
#include "Equipment/EquipmentContainer.h"

void UEquipSlotBase::Equip(UObject* Instigator, UContainerItemBase* ContainerItemToEquip)
{
    if (IsValid(ContainerItemToEquip))
    {
        if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItemToEquip->GetItem()))
        {
            EquipableItem->TakeOff();
        }
    }

    ContainerItem = ContainerItemToEquip;
    ContainerItem->Container = EquipmentContainer;

    if (IEquipableInterface* EquipableItem = Cast<IEquipableInterface>(ContainerItem->GetItem()))
    {
        EquipableItem->Equip(Instigator);
    }

    OnEquiped.Broadcast();
}

bool UEquipSlotBase::CanEquip(UItemData* ItemData)
{
    return !IsValid(ContainerItem) && ItemData->Type.HasTag(RequiredItemType);
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

    OnTakeOff.Broadcast();
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
