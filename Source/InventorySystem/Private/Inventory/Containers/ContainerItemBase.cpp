#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"
#include "Inventory/Containers/ItemsContainerBase.h"

UContainerItemBase* UContainerItemBase::ConstructContainerItem(UObject* Outer, UItemData* ItemData, TSubclassOf<UContainerItemBase> ContainerItemClass)
{
    UContainerItemBase* ContainerItemBase = NewObject<UContainerItemBase>(Outer, ContainerItemClass);
    ContainerItemBase->SetItemData(ItemData);

    return ContainerItemBase;
}

void UContainerItemBase::Swap(UContainerItemBase* OtherContainerItem)
{
    UContainerItemBase* Temp = DuplicateObject(OtherContainerItem, NULL);
    OtherContainerItem->Item = Item;
    OtherContainerItem->SetItemData(ItemData, Amount);

    Item = Temp->Item;
    SetItemData(Temp->GetItemData(), Temp->GetAmount());

}

void UContainerItemBase::AddItem(UItemBase* InItem, int32 InAmount)
{
    if (!IsValid(Item))
    {
        Item = InItem;
    }

    AddItemData(Item->ItemData, InAmount);
}

void UContainerItemBase::AddItemData(UItemData* InItemData, int32 InAmount)
{
    if (!IsValid(ItemData))
    {
        ItemData = InItemData;
    }
    else if (ItemData->ID != InItemData->ID)
    {
        checkf(InItemData->ID == InItemData->ID, TEXT("Cannot add the item with a different ID to the container item"));
        return;
    }

    SetAmount(Amount + InAmount);

}

void UContainerItemBase::SetItem(UItemBase* NewItem, int32 InAmount)
{
    Item = NewItem;
    SetItemData(Item->ItemData, InAmount);
}

void UContainerItemBase::SetItemData(UItemData* InItemData, int32 InAmount)
{
    ItemData = InItemData;
    Amount = InAmount;

    OnContainerItemUpdated.Broadcast(this);
}

void UContainerItemBase::Drop(int32 AmountToDrop)
{
    Remove(AmountToDrop);

    ReceiveDrop(AmountToDrop);
}

void UContainerItemBase::DropAll()
{
    Drop(Amount);
}

void UContainerItemBase::Remove(int32 AmountToRemove)
{
    Amount -= FMath::Clamp(Amount - AmountToRemove, 0, Amount);

    if (Amount == 0)
    {
        RemoveFromContainer();
    }

    ReceiveRemove(AmountToRemove);
}

void UContainerItemBase::RemoveAll()
{
    Remove(Amount);
}

void UContainerItemBase::RemoveFromContainer()
{
    if (IsValid(Container))
    {
        Container->RemoveContainerItem(this);
    }
}

int32 UContainerItemBase::GetAmount()
{
    return Amount;
}

void UContainerItemBase::SetAmount(int32 NewAmount)
{
    if (IsValid(ItemData))
    {
        Amount = FMath::Clamp(NewAmount, 0, ItemData->StackSize);

        checkf(NewAmount <= ItemData->StackSize, TEXT("Items stack overflow"));

        if (Amount == 0)
        {
            RemoveFromContainer();
        }
    }
}

UItemBase* UContainerItemBase::GetItem()
{
    if (!IsValid(Item))
    {
        Item = ItemData->ConstructItemInstance(this);
    }
    else if (!IsValid(Item->ItemData))
    {
        Item->ItemData = GetItemData();
    }

    return Item;
}

UItemData* UContainerItemBase::GetItemData()
{
    return ItemData;
}

bool UContainerItemBase::CanAddItem()
{
    return !IsValid(ItemData) || Amount < ItemData->StackSize;
}

bool UContainerItemBase::IsItemSameType(UContainerItemBase* OtherItem)
{
    return OtherItem->GetItemData()->ID == GetItemData()->ID;
}

bool UContainerItemBase::MergeWithOther(UContainerItemBase* OtherContainerItem)
{
    if (IsItemSameType(OtherContainerItem))
    {
        int32 Reminder = 0;
        int32 NewAmount = OtherContainerItem->Amount + Amount;
        if (NewAmount > ItemData->StackSize)
        {
            Reminder = NewAmount - ItemData->StackSize;
        }

        OtherContainerItem->SetAmount(Reminder);
        SetAmount(NewAmount - Reminder);

        return Reminder == 0;
    }

    checkf(false, TEXT("Failed merge item with different ID"));

    return false;
}

void UContainerItemBase::MergeOrSwap(UContainerItemBase* OtherContainerItem)
{
    if (IsItemSameType(OtherContainerItem))
    {
        MergeWithOther(OtherContainerItem);
    }
    else
    {
        Swap(OtherContainerItem);
    }
}

UContainerItemBase* UContainerItemBase::Split(int32 SplitAmount)
{
    if (SplitAmount > 0 && SplitAmount < Amount)
    {
        UContainerItemBase* NewContainerItem = DuplicateObject<UContainerItemBase>(this, GetOuter());
        NewContainerItem->SetAmount(SplitAmount);

        SetAmount(Amount - SplitAmount);

        return NewContainerItem;
    }

    checkf(false, TEXT("Invalid SplitAmount value: %d"), SplitAmount);

    return nullptr;
}

