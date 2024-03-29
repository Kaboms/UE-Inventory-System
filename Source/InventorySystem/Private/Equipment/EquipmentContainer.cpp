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
    while (true)
    {
        if (UEquipSlotBase* EquipSlot = FindFreeEquipSlot(ContainerItem))
        {
            if (EquipSlot->Equip(Owner, ContainerItem))
            {
                OnEquipedToSlot.Broadcast(EquipSlot->SlotTag);
                return true;
            }
        }
        else
        {
            break;
        }
    }
    return false;
}

bool UEquipmentContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    if (UEquipSlotBase* EquipSlot = GetEquipSlot(ContainerItem))
    {
        EquipSlot->TakeOff();
        OnTakeOffFromSlot.Broadcast(EquipSlot->SlotTag);
        return true;
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

bool UEquipmentContainer::CanSwapItems(UContainerItemBase* ContainerItem, UContainerItemBase* OtherItem)
{
    if (UEquipSlotBase* EquipSlot = GetEquipSlot(ContainerItem))
    {
        return EquipSlot->CanEquip(OtherItem->GetItemData());
    }
    return false;
}

UEquipSlotBase* UEquipmentContainer::FindFreeEquipSlot(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->CanEquip(ContainerItem->GetItemData()))
        {
            return SlotPair.Value;
        }
    }

    return nullptr;
}

UEquipSlotBase* UEquipmentContainer::GetEquipSlot(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->ContainerItem == ContainerItem)
        {
            return SlotPair.Value;
        }
    }

    return nullptr;
}

