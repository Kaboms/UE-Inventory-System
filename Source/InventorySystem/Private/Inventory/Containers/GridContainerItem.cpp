#include "Inventory/Containers/GridContainerItem.h"
#include "Inventory/Containers/GridItemsContainer.h"

UGridContainerItem* UGridContainerItem::ConstructFromContainerItem(UGridItemsContainer* Container, UContainerItemBase* ContainerItem)
{
    UGridContainerItem* GridContainerItem = Cast<UGridContainerItem>(ContainerItem);
    if (!GridContainerItem)
    {
        GridContainerItem = NewObject<UGridContainerItem>(Container);
        GridContainerItem->SetItem(ContainerItem->GetItem(), ContainerItem->GetAmount());
    }

    GridContainerItem->Container = Container;

    return GridContainerItem;
}