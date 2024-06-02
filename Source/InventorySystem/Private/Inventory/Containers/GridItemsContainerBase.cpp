#include "Inventory/Containers/GridItemsContainerBase.h"
#include "Inventory/Containers/ContainerItemBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/ItemData.h"

void UGridItemsContainerBase::Refill()
{
    // Override in childs
}

bool UGridItemsContainerBase::Move(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition)
{
    // Override in childs
    return true;
}

void UGridItemsContainerBase::SwapItemsPositions(FVector2f A, FVector2f B)
{
    // Override in childs
}

bool UGridItemsContainerBase::IsPositionValid(FVector2f Position)
{
    return (MaxCollumns == 0 || Position.X < MaxCollumns) && (MaxRows == 0 || Position.Y < MaxRows);
}

bool UGridItemsContainerBase::FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos)
{
    // Override in childs
    return false;
}