#include "Inventory/InventoryComponentBase.h"
#include "Inventory/ItemBase.h"
#include "Inventory/Containers/ItemsContainerBase.h"

void UInventoryComponentBase::InitializeComponent()
{
    Super::InitializeComponent();

    if (IsValid(ItemsContainer))
    {
        ItemsContainer->OnOpen.AddDynamic(this, &UInventoryComponentBase::OnContainerOpened);
        ItemsContainer->OnClose.AddDynamic(this, &UInventoryComponentBase::OnContainerOpened);
    }
}

void UInventoryComponentBase::OnContainerOpened()
{
    OnOpen.Broadcast();
}

void UInventoryComponentBase::OnContainerClosed()
{
    OnClose.Broadcast();
}
