#include "Equipment/EquipmentContainer.h"
#include "Equipment/EquipSlotBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"
#include "Inventory/Containers/ContainerItemBase.h"

void UEquipmentContainer::Init()
{
    for (auto& Slot : InstancedSlots)
    {
        Slot->EquipmentContainer = this;

        Slots.Add(Slot->SlotTag, Slot);
    }
}

bool UEquipmentContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->CanEquip(ContainerItem->GetItemData()))
        {
            SlotPair.Value->Equip(Owner, ContainerItem);
            OnEquipedToSlot.Broadcast(SlotPair.Key);
            return true;
        }
    }

    return false;
}

bool UEquipmentContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->ContainerItem == ContainerItem)
        {
            SlotPair.Value->TakeOff();
            OnTakeOffFromSlot.Broadcast(SlotPair.Key);
            return true;
        }
    }
    return false;
}

TArray<UContainerItemBase*> UEquipmentContainer::GetContainerItems()
{
    TArray<UContainerItemBase*> Result;

    for (auto& SlotPair : Slots)
    {
        UContainerItemBase* ContainerItem = SlotPair.Value->ContainerItem;
        if (IsValid(ContainerItem))
        {
            Result.Add(ContainerItem);
        }
    }

    return Result;
}

bool UEquipmentContainer::MoveToSlot(UEquipSlotBase* ContainerItemSlot, UEquipSlotBase* NewSlot)
{
    UContainerItemBase* ContainerItem = ContainerItemSlot->ContainerItem;
    if (IsValid(ContainerItem))
    {
        UContainerItemBase* NewSlotContainerItem = NewSlot->ContainerItem;
        if (IsValid(NewSlotContainerItem))
        {
            NewSlotContainerItem->MergeOrSwap(ContainerItem);

            // If not all item merged we should return it to previous pos
            return ContainerItem->GetAmount() == 0;
        }
        else if (NewSlot->CanEquip(ContainerItem->GetItemData()))
        {
            ContainerItemSlot->TakeOff();
            NewSlot->Equip(Owner, ContainerItem);
            return true;
        }
    }

    return false;
}
