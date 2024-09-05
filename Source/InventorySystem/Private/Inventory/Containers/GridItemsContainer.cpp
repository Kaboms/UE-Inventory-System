#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

void UGridItemsContainer::InitDefaultItems()
{
    FVector2f Position(0, 0);

    for (UContainerItemBase* DefaultItem : DefaultContainerItems)
    {
        // TODO Default item amount can be greater than item max stack size.
        DefaultItem->ClampAmount(DefaultItem->GetAmount());

        if (!AddContainerItemFromPosition(DefaultItem, Position))
        {
            checkf(false, TEXT("Failed to fill container with default items. Is there enough space in the container?"))
        }
    }
}

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