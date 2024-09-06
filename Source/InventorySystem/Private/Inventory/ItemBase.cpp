#include "Inventory/ItemBase.h"

void UItemBase::Init()
{
    ReceiveInit();
}

bool UItemBase::IsDroppable()
{
    return ReceiveIsDroppable();
}

bool UItemBase::DoesItemClassImplementInterface(TSubclassOf<UItemBase> ItemClass, TSubclassOf<UInterface> Interface)
{
    return ItemClass->ImplementsInterface(Interface);
}

bool UItemBase::CanMergeWithItem(UItemBase* OtherItem)
{
    return true;
}
