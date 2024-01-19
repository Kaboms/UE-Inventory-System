#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

bool UGridItemsContainer::AddItem(UItemBase* Item)
{
    FVector2f Position(0, 0);
    return AddContainerItemFromPosition(CreateContainerItem(Item), Position);
}

bool UGridItemsContainer::AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position)
{
    while (ItemsMap.Contains(Position))
    {
        UContainerItemBase* ExistedContainerItem = ItemsMap[Position];
        if (ExistedContainerItem->GetItemData()->ID == ContainerItem->GetItemData()->ID)
        {
            if (ExistedContainerItem->MergeWithOther(ContainerItem))
            {
                return true;
            }
        }

        if (!MoveToFillDirection(Position))
        {
            return false;
        }
    }

    ItemsMap.Add(Position, ContainerItem);

    return true;
}

bool UGridItemsContainer::AddItems(TArray<UItemBase*> Items)
{
    FVector2f Position(0, 0);

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
    FVector2f Position(0, 0);
    AddContainerItemFromPosition(ContainerItem, Position);
}

void UGridItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f ItemPosition;

    if (FindContainerItemPosition(ContainerItem, ItemPosition))
    {
        ItemsMap.Remove(ItemPosition);
    }
}

void UGridItemsContainer::InitDefaultItems()
{
    FVector2f Position(0, 0);

    for (FDefaultContainerItem DefaultItem : DefaultItems)
    {
        checkf(AddContainerItemFromPosition(CreateItemFromDefault(DefaultItem), Position), TEXT("Failed to fill container with default items. Is there enough space in the container?"))
    }
}

TArray<UContainerItemBase*> UGridItemsContainer::GetContainerItems()
{
    TArray<UContainerItemBase*> Result;
    ItemsMap.GenerateValueArray(Result);

    return Result;
}

void UGridItemsContainer::Refill()
{
    TArray<UContainerItemBase*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    ItemsMap.Empty();

    FVector2f Position(0, 0);

    for (UContainerItemBase* ContainerItem : ContainerItems)
    {
        ItemsMap.Add(Position, ContainerItem);
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

bool UGridItemsContainer::MoveItemToPosition(FVector2f& ItemPosition, FVector2f NewItemPosition)
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

bool UGridItemsContainer::IsPositionValid(FVector2f Position)
{
    return (MaxCollumns == 0 || Position.X < MaxCollumns) && (MaxRows == 0 || Position.Y < MaxRows);
}

bool UGridItemsContainer::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos)
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
