#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

bool UGridItemsContainer::AddContainerItems(TArray<UContainerItemBase*> ContainerItems)
{
    FVector2f Position(0, 0);
    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        if (!AddContainerItemFromPosition(ContainerItem, Position))
        {
            return false;
        }
    }

    return true;
}

bool UGridItemsContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f Position(0, 0);
    return AddContainerItemFromPosition(ContainerItem, Position);
}

bool UGridItemsContainer::AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position)
{
    if (!IsPositionValid(Position))
        return false;

    if (!ItemsMap.Contains(Position))
    {
        ItemsMap.Add(Position, ContainerItem);
        ItemsPositions.Add(ContainerItem, Position);
    }
    else
    {
        return MergeItem(ItemsMap[Position], ContainerItem);
    }

    ContainerItem->Container = this;

    OnGridContainerItemAdded.Broadcast(Position);
    OnContainerItemAdded.Broadcast(ContainerItem);
    return true;
}

void UGridItemsContainer::RemoveContainerItemFromPosition(FVector2f Position)
{
    UContainerItemBase* ContainerItem = ItemsMap[Position];
    ItemsMap.Remove(Position);
    ItemsPositions.Remove(ContainerItem);

    OnGridContainerItemRemoved.Broadcast(Position);
    OnContainerItemRemoved.Broadcast(ContainerItem);
}

bool UGridItemsContainer::AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position)
{
    while (FindFreePosition(ContainerItem, Position))
    {
        if (AddContainerItemToPosition(ContainerItem, Position))
        {
            return true;
        }
    }

    return false;
}

bool UGridItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f Position;

    if (FindContainerItemPosition(ContainerItem, Position))
    {
        RemoveContainerItemFromPosition(Position);
        return true;
    }

    return false;
}

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

TArray<UContainerItemBase*> UGridItemsContainer::GetContainerItems()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    return ContainerItems;
}

void UGridItemsContainer::Refill()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    ItemsMap.Empty();
    ItemsPositions.Empty();

    FVector2f Position(0, 0);

    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        AddContainerItemToPosition(ContainerItem, Position);
        MoveToFillDirection(Position);
    }
}

bool UGridItemsContainer::MoveToFillDirection(FVector2f& CurrentPosition)
{
    if (FillDirection == EGridContainerDirection::Horizontal)
    {
        Move(CurrentPosition.Y, MaxRows, CurrentPosition.X, MaxCollumns);
    }
    else if (FillDirection == EGridContainerDirection::Vertical)
    {
        Move(CurrentPosition.X, MaxCollumns, CurrentPosition.Y, MaxRows);
    }

    return true;
}

bool UGridItemsContainer::Move(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition)
{
    XPosition += 1;

    if (XMaxPosition != 0 && XPosition >= XMaxPosition)
    {
        if (YMaxPosition != 0 && YPosition + 1 >= YMaxPosition)
        {
            return false;
        }
        YPosition += 1;
        XPosition = 0;
    }

    return true;
}

bool UGridItemsContainer::MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition)
{
    FVector2f PrevPos;
    FindContainerItemPosition(ContainerItem, PrevPos);

    return MoveItemByPosition(PrevPos, NewItemPosition);
}

bool UGridItemsContainer::MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition)
{
    UContainerItemBase* ContainerItem = ItemsMap[ItemPosition];
    if (IsPositionValid(NewItemPosition))
    {
        if (AddContainerItemToPosition(ContainerItem, NewItemPosition))
        {
            //New position was empty and item added to is succesfull. Remove item from previous pos
            ItemsMap.Remove(ItemPosition);
            OnGridContainerItemRemoved.Broadcast(ItemPosition);
        }
        else
        {
            UContainerItemBase* OtherContainerItem = ItemsMap[NewItemPosition];
            OtherContainerItem->MergeOrSwap(ContainerItem);

            // If not all item merged we should return it to previous pos
            return ContainerItem->GetAmount() == 0;
        }
    }

    return false;
}

void UGridItemsContainer::SwapItemsPositions(FVector2f A, FVector2f B)
{
    ensure(ItemsMap.Contains(A) && ItemsMap.Contains(B));

    UContainerItemBase* AItem = ItemsMap[A];
    UContainerItemBase* BItem = ItemsMap[B];

    AItem->Swap(BItem);

    ItemsPositions[AItem] = B;
    ItemsPositions[BItem] = A;
}

bool UGridItemsContainer::FindFreePosition(UContainerItemBase* ContainerItem, FVector2f& Position)
{
    while (ItemsMap.Contains(Position))
    {
        UContainerItemBase* ExistedContainerItem = ItemsMap[Position];
        if (ExistedContainerItem->CanMergeWithItem(ContainerItem))
        {
            return true;
        }

        if (!MoveToFillDirection(Position))
        {
            return false;
        }
    }

    return true;
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