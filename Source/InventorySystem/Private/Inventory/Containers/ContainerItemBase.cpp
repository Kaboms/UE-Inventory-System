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

void UContainerItemBase::PostLoad()
{
    Super::PostLoad();
    
#if WITH_EDITOR
    UpdateItemStackSize();
#endif // WITH_EDITOR
}

bool UContainerItemBase::Swap(UContainerItemBase* OtherContainerItem)
{
    if (Container->CanSwapItems(this, OtherContainerItem) && OtherContainerItem->Container->CanSwapItems(OtherContainerItem, this))
    {
        UContainerItemBase* Temp = DuplicateObject(OtherContainerItem, NULL);
        OtherContainerItem->Item = Item;
        OtherContainerItem->SetItemData(ItemData, Amount);

        Item = Temp->Item;
        SetItemData(Temp->GetItemData(), Temp->GetAmount());

        return true;
    }

    return false;
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
    SetAmount(InAmount);

    OnUpdated.Broadcast(this);
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
    SetAmount(Amount - AmountToRemove);

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

int32 UContainerItemBase::GetAmount() const
{
    return Amount;
}

void UContainerItemBase::SetAmount(int32 NewAmount)
{
    if (IsValid(ItemData))
    {
        ClampAmount(NewAmount);

        checkf(NewAmount <= ItemData->StackSize, TEXT("Items stack overflow"));

        OnAmountChanges.Broadcast(this, Amount);

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

UItemData* UContainerItemBase::GetItemData() const
{
    return ItemData;
}

bool UContainerItemBase::CanAddItem()
{
    return !IsValid(ItemData) || Amount < ItemData->StackSize;
}

bool UContainerItemBase::CanMergeWithItem(UContainerItemBase* OtherItem)
{
    return IsItemSameType(OtherItem) && (!IsValid(Item) || Item->CanMergeWithItem(OtherItem->Item));
}

bool UContainerItemBase::IsItemSameType(UContainerItemBase* OtherItem)
{
    return OtherItem->GetItemData()== GetItemData();
}

void UContainerItemBase::ClampAmount(int32 InAmount)
{
    Amount = FMath::Clamp(InAmount, 0, ItemData->StackSize);
}

bool UContainerItemBase::MergeWithOther(UContainerItemBase* OtherContainerItem)
{
    checkf(IsItemSameType(OtherContainerItem), TEXT("Failed merge item with different ID"));

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

bool UContainerItemBase::MergeOrSwap(UContainerItemBase* OtherContainerItem)
{
    if (CanMergeWithItem(OtherContainerItem))
    {
        return MergeWithOther(OtherContainerItem);
    }
    else
    {
        Swap(OtherContainerItem);
        return true;
    }

    return false;
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

#if WITH_EDITOR
void UContainerItemBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    UpdateItemStackSize();
}

void UContainerItemBase::UpdateItemStackSize()
{
    if (IsValid(ItemData))
    {
        ItemStackSize = ItemData->StackSize;
    }
}
#endif