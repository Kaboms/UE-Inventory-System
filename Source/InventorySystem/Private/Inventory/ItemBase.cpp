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

FText UItemBase::GetDisplayName_Implementation()
{
    if (IsValid(ItemData))
    {
        return ItemData->DisplayName;
    }
    return FText();
}

FText UItemBase::GetDescription_Implementation()
{
    if (IsValid(ItemData))
    {
        return ItemData->Description;
    }
    return FText();
}

UTexture2D* UItemBase::GetIcon_Implementation()
{
    if (IsValid(ItemData))
    {
        return ItemData->Icon;
    }
    return nullptr;
}