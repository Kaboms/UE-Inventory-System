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

bool UEquipmentContainer::AddItem(UItemBase* Item)
{
    Super::AddItem(Item);

    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->CanEquip(Item->ItemData))
        {
            SlotPair.Value->Equip(Owner, CreateContainerItem(Item));
            return true;
        }
    }

    return false;
}

void UEquipmentContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->CanEquip(ContainerItem->GetItemData()))
        {
            SlotPair.Value->Equip(Owner, ContainerItem);
            OnEquipedToSlot.Broadcast(SlotPair.Key);
            break;
        }
    }
}

void UEquipmentContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    for (auto& SlotPair : Slots)
    {
        if (SlotPair.Value->ContainerItem == ContainerItem)
        {
            SlotPair.Value->TakeOff();
            OnTakeOffFromSlot.Broadcast(SlotPair.Key);
            break;
        }
    }
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
