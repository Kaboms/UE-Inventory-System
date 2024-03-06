#include "Inventory/Containers/GridItemsContainer.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

PRAGMA_DISABLE_OPTIMIZATION

bool UGridItemsContainer::AddItem(UItemBase* Item)
{
    FVector2f Position(0, 0);

    return AddContainerItemFromPosition(Item, Position);
}

bool UGridItemsContainer::AddContainerItemFromPosition(UItemBase* Item, FVector2f& Position)
{
    UContainerItemBase* ContainerItemBase = CreateContainerItem(Item);

    return AddContainerItemFromPosition(ContainerItemBase, Position);
}

bool UGridItemsContainer::FindFreePosition(UItemBase* Item, FVector2f& Position)
{
    while (ItemsMap.Contains(Position))
    {
        UContainerItemBase* ExistedContainerItem = ItemsMap[Position];
        if (ExistedContainerItem->CanAddItem() && ExistedContainerItem->GetItemData()->ID == Item->ItemData->ID)
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

bool UGridItemsContainer::AddItems(TArray<UItemBase*> Items)
{
    FVector2f Position(0, 0);

    for (UItemBase* Item : Items)
    {
        if (!AddContainerItemFromPosition(Item, Position))
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

bool UGridItemsContainer::AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position)
{
    if (!IsPositionValid(Position))
        return false;

    if (!ItemsMap.Contains(Position))
    {
        ItemsMap.Add(Position, ContainerItem);
    }
    else
    {
        UContainerItemBase* OtherContainerItem = ItemsMap[Position];
        if (OtherContainerItem->GetItemData()->ID == ContainerItem->GetItemData()->ID)
        {
            return OtherContainerItem->MergeWithOther(ContainerItem);
        }
        else
        {
            return false;
        }
    }

    ContainerItem->Container = this;

    OnContainerItemAdded.Broadcast(Position);
    return true;
}

void UGridItemsContainer::RemoveContainerItemFromPosition(FVector2f Position)
{
    UContainerItemBase* ContainerItem = ItemsMap[Position];
    ItemsMap.Remove(Position);

    OnContainerItemRemoved.Broadcast(Position);
}

bool UGridItemsContainer::AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position)
{
    while (FindFreePosition(ContainerItem->GetItem(), Position))
    {
        if (AddContainerItemToPosition(ContainerItem, Position))
        {
            return true;
        }
    }

    return false;
}

void UGridItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    FVector2f Position;

    FindContainerItemPosition(ContainerItem, Position);

    RemoveContainerItemFromPosition(Position);
}

void UGridItemsContainer::InitDefaultItems()
{
    FVector2f Position(0, 0);

    for (UContainerItemBase* DefaultItem : DefaultContainerItems)
    {
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
            RemoveContainerItemFromPosition(ItemPosition);
        }
        else
        {
            UContainerItemBase* OtherContainerItem = ItemsMap[NewItemPosition];
            if (OtherContainerItem->GetItemData()->ID == ContainerItem->GetItemData()->ID)
            {
                // Items merged so we do nothing
                return false;
            }
            else
            {
                // It's different items so we swap them
                SwapItems(ContainerItem, OtherContainerItem);
            }
        }
    }

    return true;
}

void UGridItemsContainer::SwapItemsPositions(FVector2f A, FVector2f B)
{
    ensure(ItemsMap.Contains(A) && ItemsMap.Contains(B));

    SwapItems(ItemsMap[A], ItemsMap[B]);
}

void UGridItemsContainer::SwapItems(UContainerItemBase* A, UContainerItemBase* B)
{
    A->Swap(B);
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

PRAGMA_ENABLE_OPTIMIZATION
