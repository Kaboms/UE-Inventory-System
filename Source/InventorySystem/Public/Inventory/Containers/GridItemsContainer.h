#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/GridItemsContainerBase.h"

#include "GridItemsContainer.generated.h"

/**
 * Grid containers with free or fixed items positions.
 * Each item has same square size. Use UTetrisItemsContainer for items with different rect sizes and custom form.
 */
UCLASS()
class INVENTORYSYSTEM_API UGridItemsContainer : public UGridItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos) override;

	// Refiil grid to remove empty spaces
	virtual void Refill() override;

	// Swap items within same container.
	virtual void SwapItemsPositions(FVector2f A, FVector2f B) override;

protected:
	virtual void HandleContainerItemRemoved(UContainerItemBase* ContainerItem) override;

	virtual void HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position);

private:
	// For fast access to items positions
	TMap<UContainerItemBase*, FVector2f> ItemsPositions;
};
