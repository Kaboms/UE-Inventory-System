#include "Inventory/Containers/TetrisItemsContainer.h"

void UTetrisItemsContainer::InitDefaultItems()
{
}

void UTetrisItemsContainer::PlaceToCell(UContainerItemBase* ContainerItem, FVector2f SelectedSlotCell, FVector2f TargetSlotCell)
{
    if (UItemMetadata* ItemMetadata = ContainerItem->GetItemData()->FindMetadataByClass(UTetrisContainerSlotMetadata::StaticClass()))
    {
        UTetrisContainerSlotMetadata* SlotMetadata = Cast<UTetrisContainerSlotMetadata>(ItemMetadata);

        FVector2f TopLeft = TargetSlotCell - SelectedSlotCell;

        for (int32 x = 0; x < SlotMetadata->Size.X; x++)
        {
            for (int32 y = 0; y < SlotMetadata->Size.Y; y++)
            {
                FVector2f CellToAdd = TopLeft + FVector2f(x, y);
                ItemsMap.Add(CellToAdd, ContainerItem);
            }
        }
    }
}

bool UTetrisItemsContainer::CanPlaceTo(UTetrisContainerSlotMetadata* SlotMetadata, FVector2f SelectedSlotCell, FVector2f TargetSlotCell, TArray<FVector2f>& OutOccupiedCells)
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
