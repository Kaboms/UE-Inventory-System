// (c) 2021 Sonic Terror

#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"
#include "Inventory/Containers/ItemsContainerBase.h"

void UContainerItemBase::AddItem(UItemBase* InItem, int32 InAmount)
{
    if (!IsValid(Item))
    {
        Item = InItem;
    }
    else
    {
        checkf(Item->ItemData->ID == InItem->ItemData->ID, TEXT("Cannot add the item with a different ID to the container item"));
    }

    if (Amount + InAmount <= Item->ItemData->StackSize)
    {
        Amount += InAmount;
    }
    else
    {
        checkNoEntry()
    }
}

void UContainerItemBase::SetItem(UItemBase* NewItem, int32 InAmount)
{
    Item = NewItem;
    Amount = InAmount;
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
        Container->RemoveContainerItem(this);
    }

    ReceiveRemove(AmountToRemove);
}

void UContainerItemBase::RemoveAll()
{
    Remove(Amount);
}

int32 UContainerItemBase::GetAmount()
{
    return Amount;
}

void UContainerItemBase::SetAmount(int32 NewAmount)
{
    if (IsValid(Item))
    {
        Amount = FMath::Clamp(NewAmount, 0, Item->ItemData->StackSize);
        if (Amount == 0)
        {
            RemoveAll();
        }
    }
}

UItemBase* UContainerItemBase::GetItem()
{
    return Item;
}

bool UContainerItemBase::MergeWithOther(UContainerItemBase* OtherContainerItem)
{
    checkf(Item->ItemData->ID == OtherContainerItem->GetItem()->ItemData->ID, TEXT("Failed merge item with different ID"));

    int32 Reminder = 0;
    int32 NewAmount = OtherContainerItem->Amount + Amount;
    if (NewAmount > Item->ItemData->StackSize)
    {
        Reminder = NewAmount - Item->ItemData->StackSize;
    }

    OtherContainerItem->SetAmount(Reminder);
    SetAmount(NewAmount - Reminder);

    return Reminder == 0;
}

UContainerItemBase* UContainerItemBase::Split(int32 SplitAmount)
{
    checkf(SplitAmount > 0 && SplitAmount < Amount, TEXT("Invalid SplitAmount value: %d"), SplitAmount);

    UContainerItemBase* NewContainerItem = DuplicateObject<UContainerItemBase>(this, GetOuter());
    NewContainerItem->SetAmount(SplitAmount);

    SetAmount(Amount - SplitAmount);

    return NewContainerItem;
}

