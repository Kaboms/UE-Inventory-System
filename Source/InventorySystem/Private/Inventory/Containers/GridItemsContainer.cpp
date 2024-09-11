#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

void UGridItemsContainer::Refill()
{
    ItemsPositions.Empty();

    Super::Refill();
}

void UGridItemsContainer::SwapItemsPositions(FVector2f A, FVector2f B)
{
    UContainerItemBase* AItem = ItemsMap[A];
    UContainerItemBase* BItem = ItemsMap[B];

    Super::SwapItemsPositions(A, B);

    ItemsPositions[AItem] = B;
    ItemsPositions[BItem] = A;
}

void UGridItemsContainer::HandleContainerItemRemoved(UContainerItemBase* ContainerItem)
{
    ItemsPositions.Remove(ContainerItem);

    Super::HandleContainerItemRemoved(ContainerItem);
}

void UGridItemsContainer::HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position)
{
    Super::HandleAddContainerItemToPosition(ContainerItem, Position);

    ItemsPositions.Add(ContainerItem, Position);
}

bool UGridItemsContainer::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos)
{
    if (FVector2f* ItemPos = ItemsPositions.Find(ContainerItem))
    {
        OutPos = *ItemPos;
        return true;
    }

    return false;
}