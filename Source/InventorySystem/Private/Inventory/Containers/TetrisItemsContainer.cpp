#include "Inventory/Containers/TetrisItemsContainer.h"

void UTetrisItemsContainer::InitDefaultItems()
{
    TArray<UContainerItemBase*> SortedDefaultItems = DefaultContainerItems;

    SortItemsBySlotSize(SortedDefaultItems);

    if (AddContainerItems(SortedDefaultItems))
    {
        checkf(false, TEXT("Failed to fill container with default items. Is there enough space in the container?"))
    }
}

bool UTetrisItemsContainer::AddContainerItems(TArray<UContainerItemBase*> ContainerItems)
{
    return false;
}

bool UTetrisItemsContainer::AddContainerItem(UContainerItemBase* ContainerItem)
{
    return false;
}

bool UTetrisItemsContainer::RemoveContainerItem(UContainerItemBase* ContainerItem)
{
    return false;
}

bool UTetrisItemsContainer::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos)
{
    if (FTetrisContainerSlot* TetrisContainer = ItemsSlots.Find(ContainerItem))
    {
        OutPos = TetrisContainer->Position;
        return true;
    }
    return false;
}

void UTetrisItemsContainer::SortItemsBySlotSize(TArray<UContainerItemBase*>& ItemsToSort)
{
    // Sort by slot sizes
    ItemsToSort.Sort([](const UContainerItemBase& A, const UContainerItemBase& B)
        {
            UTetrisContainerSlotMetadata* ASlotMetadata = UTetrisContainerSlotMetadata::FindMetadata(A.GetItemData());
            UTetrisContainerSlotMetadata* BSlotMetadata = UTetrisContainerSlotMetadata::FindMetadata(B.GetItemData());

            return ASlotMetadata->Size.ComponentwiseAllGreaterThan(BSlotMetadata->Size);
        });
}

void UTetrisItemsContainer::PlaceToCell(UContainerItemBase* ContainerItem, FVector2f SelectedSlotCell, FVector2f TargetSlotCell)
{
    if (UTetrisContainerSlotMetadata* SlotMetadata = UTetrisContainerSlotMetadata::FindMetadata(ContainerItem->GetItemData()))
    {
        FVector2f TopLeft = TargetSlotCell - SelectedSlotCell;

        FTetrisContainerSlot Slot;
        Slot.ContainerItem = ContainerItem;
        Slot.SlotMetadata = SlotMetadata;
        Slot.Position = TopLeft;

        for (int32 x = 0; x < SlotMetadata->Size.X; x++)
        {
            for (int32 y = 0; y < SlotMetadata->Size.Y; y++)
            {
                FVector2f CellToAdd = TopLeft + FVector2f(x, y);
                ItemsMap.Add(CellToAdd, Slot);
            }
        }
    }
}

bool UTetrisItemsContainer::CanPlaceTo(UTetrisContainerSlotMetadata* SlotMetadata, FVector2f SelectedSlotCell, FVector2f TargetSlotCell, TArray<FVector2f>& OutOccupiedCells) const
{
    FVector2f TopLeft = TargetSlotCell - SelectedSlotCell;

    if (TopLeft.X < 0 || TopLeft.Y < 0)
        return false;

    bool bCheckForm = !SlotMetadata->Form.IsEmpty();

    bool Result = true;

    for (int32 x = 0; x < SlotMetadata->Size.X; x++)
    {
        for (int32 y = 0; y < SlotMetadata->Size.Y; y++)
        {
            FVector2f CellToCheck = TopLeft + FVector2f(x, y);

            // Cell is already contained item
            if (ItemsMap.Contains(CellToCheck))
            {
                if (bCheckForm)
                {
                    if (!SlotMetadata->Form.Contains(CellToCheck))
                    {
                        continue;
                    }
                }

                OutOccupiedCells.Add(CellToCheck);
                Result = false;
            }
        }
    }

    return Result;
}