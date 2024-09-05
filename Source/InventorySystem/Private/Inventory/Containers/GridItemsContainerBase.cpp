#include "Inventory/Containers/GridItemsContainerBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

void UGridItemsContainerBase::Refill()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    ItemsMap.Empty();

    FVector2f Position(0, 0);

    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        AddContainerItemToPosition(ContainerItem, Position);
        MoveToFillDirection(Position);
    }
}

bool UGridItemsContainerBase::GetNextCellPosition(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition)
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

void UGridItemsContainerBase::SwapItemsPositions(FVector2f A, FVector2f B)
{
    ensure(ItemsMap.Contains(A) && ItemsMap.Contains(B));

    ItemsMap[A]->Swap(ItemsMap[B]);
}

bool UGridItemsContainerBase::IsPositionValid(FVector2f Position) const
{
    return (MaxCollumns == 0 || Position.X < MaxCollumns) && (MaxRows == 0 || Position.Y < MaxRows);
}

bool UGridItemsContainerBase::CanAddToPosition(UContainerItemBase* ContainerItem, FVector2f Position) const
{
    return IsPositionValid(Position);
}

TArray<UContainerItemBase*> UGridItemsContainerBase::GetContainerItems()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    return ContainerItems;
}

bool UGridItemsContainerBase::FindFreePosition(UContainerItemBase* ContainerItem, FVector2f& Position)
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

bool UGridItemsContainerBase::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f Position;

    if (FindContainerItemPosition(ContainerItem, Position))
    {
        RemoveContainerItemFromPosition(Position);
        return true;
    }

    return false;
}

void UGridItemsContainerBase::EmptyPosition(const FVector2f& Position)
{
    ItemsMap.Remove(Position);
    OnGridContainerItemRemoved.Broadcast(Position);
}

void UGridItemsContainerBase::HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position)
{
    ItemsMap.Add(Position, ContainerItem);
}

bool UGridItemsContainerBase::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos)
{
    // Override in childs
    return false;
}

bool UGridItemsContainerBase::AddContainerItems(TArray<UContainerItemBase*> ContainerItems)
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

bool UGridItemsContainerBase::AddContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f Position(0, 0);
    return AddContainerItemFromPosition(ContainerItem, Position);
}

bool UGridItemsContainerBase::AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position)
{
    if (!CanAddToPosition(ContainerItem, Position))
        return false;

    if (!ItemsMap.Contains(Position))
    {
        HandleAddContainerItemToPosition(ContainerItem, Position);
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

void UGridItemsContainerBase::RemoveContainerItemFromPosition(FVector2f Position)
{
    UContainerItemBase* ContainerItem = ItemsMap[Position];

    EmptyPosition(Position);

    HandleContainerItemRemoved(ContainerItem);
}

bool UGridItemsContainerBase::AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position)
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

bool UGridItemsContainerBase::MoveToFillDirection(FVector2f& CurrentPosition)
{
    if (FillDirection == EGridContainerDirection::Horizontal)
    {
        GetNextCellPosition(CurrentPosition.Y, MaxRows, CurrentPosition.X, MaxCollumns);
    }
    else if (FillDirection == EGridContainerDirection::Vertical)
    {
        GetNextCellPosition(CurrentPosition.X, MaxCollumns, CurrentPosition.Y, MaxRows);
    }

    return true;
}

bool UGridItemsContainerBase::MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition)
{
    FVector2f PrevPos;
    FindContainerItemPosition(ContainerItem, PrevPos);

    return MoveItemByPosition(PrevPos, NewItemPosition);
}

bool UGridItemsContainerBase::MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition)
{
    UContainerItemBase* ContainerItem = ItemsMap[ItemPosition];
    if (CanAddToPosition(ContainerItem, NewItemPosition))
    {
        if (AddContainerItemToPosition(ContainerItem, NewItemPosition))
        {
            //New position was empty and item added to is succesfull. Remove item from previous pos
            EmptyPosition(ItemPosition);
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