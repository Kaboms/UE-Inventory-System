#include "Inventory/Containers/ItemsContainerBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"
#include "Inventory/Containers/ContainerItemBase.h"

bool UItemsContainerBase::AddItem(UItemBase* Item)
{
    UContainerItemBase* ContainerItem = CreateContainerItem(Item);
    return AddContainerItem(ContainerItem);
}

bool UItemsContainerBase::AddItems(TArray<UItemBase*> Items)
{
    for (UItemBase* Item : Items)
    {
        if (!AddItem(Item))
        {
            return false;
        }
    }

    return true;
}

bool UItemsContainerBase::AddItemData(UItemData* ItemData)
{
    UContainerItemBase* ContainerItem = CreateContainerItemFromData(ItemData);
    return AddContainerItem(ContainerItem);
}

bool UItemsContainerBase::AddItemsData(TArray<UItemData*> ItemsData)
{
    for (UItemData* ItemData : ItemsData)
    {
        if (!AddItemData(ItemData))
        {
            return false;
        }
    }

    return true;
}

void UItemsContainerBase::InitDefaultItems()
{
    for (UContainerItemBase* DefaultItem : DefaultContainerItems)
    {
        AddContainerItem(DefaultItem);
    }
}

bool UItemsContainerBase::AddContainerItem(UContainerItemBase* ContainerItem)
{
    if (ReceiveAddContainerItem(ContainerItem))
    {
        OnContainerItemAdded.Broadcast(ContainerItem);
        return true;
    }
    return false;
}

bool UItemsContainerBase::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    if (ReceiveRemoveContainerItem(ContainerItem))
    {
        HandleContainerItemRemoved(ContainerItem);
        return true;
    }
    return false;
}

bool UItemsContainerBase::AddContainerItems(TArray<UContainerItemBase*> ContainerItems)
{
    return true;
}

UContainerItemBase* UItemsContainerBase::CreateContainerItem(UItemBase* Item)
{
    UContainerItemBase* ContainerItem = NewObject<UContainerItemBase>(this);
    ContainerItem->Container = this;
    ContainerItem->SetItem(Item);

    return ContainerItem;
}

UContainerItemBase* UItemsContainerBase::CreateContainerItemFromData(UItemData* ItemData)
{
    UContainerItemBase* ContainerItem = NewObject<UContainerItemBase>(this);
    ContainerItem->Container = this;
    ContainerItem->SetItemData(ItemData);

    return ContainerItem;
}

UContainerItemBase* UItemsContainerBase::FindContainerItem(UItemData* ItemData)
{
    TArray<UContainerItemBase*> ContainerItems = GetContainerItems();

    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        if (ContainerItem->GetItemData() == ItemData)
        {
            return ContainerItem;
        }
    }

    return nullptr;
}

TArray<UContainerItemBase*> UItemsContainerBase::GetContainerItems()
{
    return TArray<UContainerItemBase*>();
}

void UItemsContainerBase::Open()
{
    ReceiveOpen();

    OnOpen.Broadcast();
}

void UItemsContainerBase::Close()
{
    ReceiveClose();

    OnClose.Broadcast();
}

bool UItemsContainerBase::CanSwapItems(UContainerItemBase* ContainerItem, UContainerItemBase* OtherItem)
{
    return true;
}

bool UItemsContainerBase::MergeItem(UContainerItemBase* ContainerItem, UContainerItemBase* OtherItem)
{
    if (ContainerItem->IsItemSameType(OtherItem))
    {
        return ContainerItem->MergeWithOther(OtherItem);
    }
    else
    {
        return false;
    }
}

bool UItemsContainerBase::IsEmpty()
{
    return GetContainerItems().Num() <= 0;
}

void UItemsContainerBase::HandleContainerItemRemoved(UContainerItemBase* ContainerItem)
{
    OnContainerItemRemoved.Broadcast(ContainerItem);
}
