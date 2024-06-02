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
	virtual bool AddContainerItems(TArray<UContainerItemBase*> ContainerItems) override;

	virtual bool AddContainerItem(UContainerItemBase* ContainerItem) override;

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveContainerItemFromPosition(FVector2f Position);

	// Find free position started from Position.
	// Return false if here is no free position to place item.
	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position);

	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	virtual void InitDefaultItems() override;

	virtual TArray<UContainerItemBase*> GetContainerItems() override;

	virtual bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos) override;

	// Refiil grid to remove empty spaces
	virtual void Refill() override;

	UFUNCTION(BlueprintCallable)
	bool MoveToFillDirection(FVector2f& CurrentPosition);

	virtual bool Move(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition) override;

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition);

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition);

	// Swap items within same container.
	virtual void SwapItemsPositions(FVector2f A, FVector2f B) override;

protected:
	bool FindFreePosition(UContainerItemBase* ContainerItem, FVector2f& Position);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2f, UContainerItemBase*> ItemsMap;

private:
	// For fast access to items positions
	TMap<UContainerItemBase*, FVector2f> ItemsPositions;
};
