// (c) 2021 Sonic Terror

#include "Inventory/ItemBase.h"

bool UItemBase::IsDroppable()
{
    return ReceiveIsDroppable();
}

bool UItemBase::DoesItemClassImplementInterface(TSubclassOf<UItemBase> ItemClass, TSubclassOf<UInterface> Interface)
{
    return ItemClass->ImplementsInterface(Interface);
}
