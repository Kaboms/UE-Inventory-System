#include "Inventory/Containers/ListItemsContainer.h"

bool UListItemsContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    if (MaxItems < 0 || Items.Num() <= MaxItems)
    {
        Items.Add(ContainerItem);
        OnContainerItemAdded.Broadcast(ContainerItem);
        return true;
    }
    return false;
}

bool UListItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    Items.Remove(ContainerItem);
    OnContainerItemRemoved.Broadcast(ContainerItem);
    return true;;
}
