#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

bool UGridItemsContainer::AddItem(UItemBase* Item)
{
    FVector2D Position(0, 0);
    return AddContainerItemFromPosition(CreateContainerItem(Item), Position);
}

bool UGridItemsContainer::AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2D& Position)
{
    while (ItemsMap.Contains(Position))
    {
        UContainerItemBase* ExistedContainerItem = ItemsMap[Position];
        if (ExistedContainerItem->GetItem()->ItemData->ID == ContainerItem->GetItem()->ItemData->ID)
        {
            if (ExistedContainerItem->MergeWithOther(ContainerItem))
            {
                return true;
            }
        }

        if (MoveToDirection(Position, FillDirection) == false)
        {
            return false;
        }
    }

    ItemsMap.Add(Position, ContainerItem);

    return true;
}

bool UGridItemsContainer::AddItems(TArray<UItemBase*> Items)
{
    FVector2D Position(0, 0);

    for (UItemBase* Item : Items)
    {
        if (!AddContainerItemFromPosition(CreateContainerItem(Item), Position))
        {
            return false;
        }
    }

    return true;
}

void UGridItemsContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2D Position(0, 0);
    AddContainerItemFromPosition(ContainerItem, Position);
}

void UGridItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2D ItemPosition;

    if (FindContainerItemPosition(ContainerItem, ItemPosition))
    {
        ItemsMap.Remove(ItemPosition);
    }
}

void UGridItemsContainer::Refill()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    ItemsMap.Empty();

    FVector2D Position(0, 0);

    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        ItemsMap.Add(Position, ContainerItem);
        MoveToDirection(Position, FillDirection);
    }
}

bool UGridItemsContainer::MoveToDirection(FVector2D& CurrentPosition, EGridContainerDirection Direction)
{
    if (FillDirection == EGridContainerDirection::Horizontal)
    {
        CurrentPosition.X += 1;

        if (MaxCollumns != 0 && CurrentPosition.X >= MaxCollumns)
        {
            if (CurrentPosition.Y + 1 >= MaxRows)
            {
                return false;
            }
            CurrentPosition.Y += 1;
            CurrentPosition.X = 0;
        }
    }
    else if (FillDirection == EGridContainerDirection::Vertical)
    {
        CurrentPosition.Y += 1;

        if (MaxRows != 0 && CurrentPosition.Y >= MaxRows)
        {
            if (CurrentPosition.X + 1 >= MaxCollumns)
            {
                return false;
            }
            CurrentPosition.Y = 0;
            CurrentPosition.X += 1;
        }
    }

    return true;
}

bool UGridItemsContainer::MoveItemToPosition(FVector2D& ItemPosition, FVector2D NewItemPosition)
{
    if (IsPositionValid(NewItemPosition) && ItemsMap.Contains(ItemPosition) && !ItemsMap.Contains(NewItemPosition))
    {
        UContainerItemBase* ContainerItemBase = ItemsMap[ItemPosition];
        ItemsMap.Remove(ItemPosition);

        ItemsMap[NewItemPosition] = ContainerItemBase;

        return true;
    }

    return false;
}

bool UGridItemsContainer::IsPositionValid(FVector2D Position)
{
    return (MaxCollumns == 0 || Position.X < MaxCollumns) && (MaxRows == 0 || Position.Y < MaxRows);
}

bool UGridItemsContainer::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2D& OutPos)
{
    for (auto ContainerItemPair : ItemsMap)
    {
        if (ContainerItemPair.Value == ContainerItem)
        {
            OutPos = ContainerItemPair.Key;
            return true;
        }
    }

    return false;
}
