#include "Inventory/Containers/TetrisItemsContainer.h"

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

bool UTetrisItemsContainer::CanAddToPosition(UContainerItemBase* ContainerItem, FVector2f Position) const
{
    if (!Super::CanAddToPosition(ContainerItem, Position))
        return false;

    UTetrisContainerSlotMetadata* TetrisSlotMetadata = UTetrisContainerSlotMetadata::FindMetadata(ContainerItem->GetItemData());

    TArray<FVector2f> OutOccupiedCells;
    return CanPlaceTo(TetrisSlotMetadata, FVector2f::ZeroVector, Position, OutOccupiedCells);
}

void UTetrisItemsContainer::SwapItemsPositions(FVector2f A, FVector2f B)
{
    UContainerItemBase* AItem = ItemsMap[A];
    UContainerItemBase* BItem = ItemsMap[B];

    FTetrisContainerSlot ASlot = ItemsSlots[AItem];
    FTetrisContainerSlot BSlot = ItemsSlots[BItem];

    Super::SwapItemsPositions(A, B);

    ItemsSlots[AItem] = BSlot;
    ItemsSlots[BItem] = ASlot;
}

void UTetrisItemsContainer::HandleContainerItemRemoved(UContainerItemBase* ContainerItem)
{
    Super::HandleContainerItemRemoved(ContainerItem);

    ItemsSlots.Remove(ContainerItem);
}

void UTetrisItemsContainer::EmptyPosition(const FVector2f& Position)
{
    FTetrisContainerSlot Slot = ItemsSlots[ItemsMap[Position]];
    for (int32 x = 0; x < Slot.SlotMetadata->Size.X; x++)
    {
        for (int32 y = 0; y < Slot.SlotMetadata->Size.Y; y++)
        {
            FVector2f CellToRemove = Slot.Position + FVector2f(x, y);
            ItemsMap.Remove(CellToRemove);
        }
    }

    OnGridContainerItemRemoved.Broadcast(Position);
}

void UTetrisItemsContainer::HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position)
{
    PlaceToCell(ContainerItem, FVector2f::ZeroVector, Position);
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

        ItemsSlots.Add(ContainerItem, Slot);

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

bool UTetrisItemsContainer::GetSlotFromPosition(FVector2f Position, FTetrisContainerSlot& OutSlot)
{
    if (UContainerItemBase** ContainerItemBasePtr = ItemsMap.Find(Position))
    {
        if (FTetrisContainerSlot* Slot = ItemsSlots.Find(*ContainerItemBasePtr))
        {
            OutSlot = *Slot;
            return true;
        }
    }

    return false;
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