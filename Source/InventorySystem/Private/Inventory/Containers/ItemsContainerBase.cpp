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
    return AddItem(UItemData::ConstructItemInstance(this, ItemData));
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
