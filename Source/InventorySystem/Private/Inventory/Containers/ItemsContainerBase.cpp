// (c) 2021 Sonic Terror

#include "Inventory/Containers/ItemsContainerBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"
#include "Inventory/Containers/ContainerItemBase.h"

bool UItemsContainerBase::AddItem(UItemBase* Item)
{
    return ReceiveAddItem(Item);
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
    UItemBase* ItemBase = NewObject<UItemBase>(this, ItemData->ItemInstanceClass);
    ItemBase->ItemData = ItemData;

    return AddItem(ItemBase);
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
    for (FDefaultContainerItem DefaultItem : DefaultItems)
    {
        UItemData* ItemData = DefaultItem.Item;

        UItemBase* ItemBase = NewObject<UItemBase>(this, ItemData->ItemInstanceClass);
        ItemBase->ItemData = ItemData;

        UContainerItemBase* ContainerItem = DefaultItem.ContainerItem;
        ContainerItem->SetItem(ItemBase, ContainerItem->GetAmount());

        AddContainerItem(ContainerItem);
    }
}

void UItemsContainerBase::AddContainerItem(UContainerItemBase* ContainerItem)
{
    ReceiveAddContainerItem(ContainerItem);
}

void UItemsContainerBase::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    ReceiveRemoveContainerItem(ContainerItem);
}

UContainerItemBase* UItemsContainerBase::CreateContainerItem(UItemBase* Item)
{
    UContainerItemBase* ContainerItem = NewObject<UContainerItemBase>(this);
    ContainerItem->Container = this;
    ContainerItem->AddItem(Item);

    return ContainerItem;
}
