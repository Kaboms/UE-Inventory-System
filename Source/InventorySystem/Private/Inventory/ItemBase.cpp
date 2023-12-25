// (c) 2021 Sonic Terror

#include "Inventory/ItemBase.h"

bool UItemBase::IsDroppable()
{
    return ReceiveIsDroppable();
}