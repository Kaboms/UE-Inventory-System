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
    UGridContainerItem* GridContainerItem = CreateGridContainerItem(Item);

    return AddGridContainerItemFromPosition(GridContainerItem, Position);
}

bool UGridItemsContainer::FindFreePosition(UItemBase* Item, FVector2f& Position)
{
    while (ItemsMap.Contains(Position))
    {
        UGridContainerItem* ExistedContainerItem = ItemsMap[Position];
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
    if (!ContainerItem)
        return;

    UGridContainerItem* GridContainerItem = UGridContainerItem::ConstructFromContainerItem(this, ContainerItem);
    AddGridContainerItem(GridContainerItem);
}

bool UGridItemsContainer::AddGridContainerItem(UGridContainerItem* GridContainerItem)
{
    if (!AddGridContainerItemToPosition(GridContainerItem, GridContainerItem->Position))
    {
        FVector2f Position(0, 0);
        return AddGridContainerItemFromPosition(GridContainerItem, Position);
    }

    return true;
}

bool UGridItemsContainer::AddGridContainerItemToPosition(UGridContainerItem* GridContainerItem, FVector2f Position)
{
    if (!IsPositionValid(Position))
        return false;

    if (!ItemsMap.Contains(Position))
    {
        GridContainerItem->Position = Position;
        ItemsMap.Add(GridContainerItem->Position, GridContainerItem);
    }
    else
    {
        UGridContainerItem* OtherContainerItem = ItemsMap[Position];
        if (OtherContainerItem->GetItemData()->ID == GridContainerItem->GetItemData()->ID)
        {
            return OtherContainerItem->MergeWithOther(GridContainerItem);
        }
        else
        {
            return false;
        }
    }

    OnContainerItemAdded.Broadcast(Position);
    return true;
}

void UGridItemsContainer::RemoveGridContainerItemFromPosition(FVector2f Position)
{
    UGridContainerItem* GridContainerItem = ItemsMap[Position];
    ItemsMap.Remove(Position);

    OnContainerItemRemoved.Broadcast(Position);
}

bool UGridItemsContainer::AddGridContainerItemFromPosition(UGridContainerItem* GridContainerItem, FVector2f& Position)
{
    while (FindFreePosition(GridContainerItem->GetItem(), Position))
    {
        if (AddGridContainerItemToPosition(GridContainerItem, Position))
        {
            return true;
        }
    }

    return false;
}

bool UGridItemsContainer::ChangeContainerItemAtPosition(FVector2f Position, UGridContainerItem* NewGridContainerItem)
{
    return false;
}

void UGridItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    if (UGridContainerItem* GridContainerItem = Cast<UGridContainerItem>(ContainerItem))
    {
        RemoveGridContainerItemFromPosition(GridContainerItem->Position);
    }
}

void UGridItemsContainer::InitDefaultItems()
{
    FVector2f Position(0, 0);

    for (UContainerItemBase* DefaultItem : DefaultContainerItems)
    {
        UGridContainerItem* GridContainerItem = UGridContainerItem::ConstructFromContainerItem(this, DefaultItem);
        if (!ItemsMap.Contains(GridContainerItem->Position))
        {
            AddGridContainerItem(GridContainerItem);
        }
        else if (!AddGridContainerItemFromPosition(GridContainerItem, Position))
        {
            checkf(false, TEXT("Failed to fill container with default items. Is there enough space in the container?"))
        }
    }
}

TArray<UContainerItemBase*> UGridItemsContainer::GetContainerItems()
{
    TArray<UContainerItemBase*> Result;

    TArray<UGridContainerItem*> GridContainerItems;
    ItemsMap.GenerateValueArray(GridContainerItems);

    for (UGridContainerItem* GridContainerItem : GridContainerItems)
    {
        Result.Add(GridContainerItem);
    }

    return Result;
}

UGridContainerItem* UGridItemsContainer::CreateGridContainerItem(UItemBase* Item)
{
    UGridContainerItem* ContainerItem = NewObject<UGridContainerItem>(this);
    ContainerItem->Container = this;
    ContainerItem->AddItem(Item);
    return ContainerItem;
}

UContainerItemBase* UGridItemsContainer::CreateContainerItem(UItemBase* Item)
{
    return CreateGridContainerItem(Item);
}

void UGridItemsContainer::Refill()
{
    TArray<UGridContainerItem*> ContainerItems;
    ItemsMap.GenerateValueArray(ContainerItems);

    ItemsMap.Empty();

    FVector2f Position(0, 0);

    for (UGridContainerItem* ContainerItem : ContainerItems)
    {
        AddGridContainerItemToPosition(ContainerItem, Position);
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

bool UGridItemsContainer::MoveItemToPosition(UGridContainerItem* GridContainerItem, FVector2f NewItemPosition)
{
    FVector2f PrevPos = GridContainerItem->Position;
    if (IsPositionValid(NewItemPosition))
    {
        if (AddGridContainerItemToPosition(GridContainerItem, NewItemPosition))
        {
            //New position was empty and item added to is succesfull. Remove item from previous pos
            RemoveGridContainerItemFromPosition(PrevPos);
        }
        else
        {
            UGridContainerItem* OtherGridContainerItem = ItemsMap[NewItemPosition];
            if (OtherGridContainerItem->GetItemData()->ID == GridContainerItem->GetItemData()->ID)
            {
                // Items merged so we do nothing
                return false;
            }
            else
            {
                // It's different items so we swap them
                SwapItems(GridContainerItem, OtherGridContainerItem);
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

void UGridItemsContainer::SwapItems(UGridContainerItem* A, UGridContainerItem* B)
{
    A->Swap(B);
}

bool UGridItemsContainer::IsPositionValid(FVector2f Position)
{
    return (MaxCollumns == 0 || Position.X < MaxCollumns) && (MaxRows == 0 || Position.Y < MaxRows);
}

bool UGridItemsContainer::FindContainerItemPosition(UGridContainerItem* ContainerItem, FVector2f& OutPos)
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
