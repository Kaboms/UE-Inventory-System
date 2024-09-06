#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/ItemsContainerBase.h"

#include "GridItemsContainerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridContainerItemAdded, FVector2f, ItemPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridContainerItemRemoved, FVector2f, ItemPos);

UENUM(BlueprintType)
enum class EGridContainerDirection : uint8
{
	Horizontal, // Column by column
	Vertical // Row by row
};

/**
 * Grid containers with free or fixed items positions.
 * Each item has same square size. Use UTetrisItemsContainer for items with different rect sizes and custom form.
 */
UCLASS(Abstract)
class INVENTORYSYSTEM_API UGridItemsContainerBase : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual bool AddContainerItems(TArray<UContainerItemBase*> ContainerItems) override;

	virtual bool AddContainerItem(UContainerItemBase* ContainerItem) override;

	// Check if we CanAddToPosition and try add item to position
	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveContainerItemFromPosition(FVector2f Position);

	// Find free position started from Position.
	// Return false if here is no free position to place item.
	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position);

	UFUNCTION(BlueprintCallable)
	virtual bool MoveToFillDirection(FVector2f& CurrentPosition);

	// Refiil grid to remove empty spaces
	UFUNCTION(BlueprintCallable)
	virtual void Refill();

	virtual bool GetNextCellPosition(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition);

	UFUNCTION(BlueprintCallable)
	virtual  bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos);

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition);

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition);

	// Swap items within same container.
	UFUNCTION(BlueprintCallable)
	virtual void SwapItemsPositions(FVector2f A, FVector2f B);

	// Is Position >= 0 and < MaxRows and MaxCollumns
	UFUNCTION(BlueprintPure)
	bool IsPositionValid(FVector2f Position) const;

	UFUNCTION(BlueprintPure)
	virtual bool CanAddToPosition(UContainerItemBase* ContainerItem, FVector2f Position) const;

	virtual TArray<UContainerItemBase*> GetContainerItems() override;

protected:
	virtual bool FindFreePosition(UContainerItemBase* ContainerItem, FVector2f& Position);

	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	virtual void EmptyPosition(const FVector2f& Position);

	// Set container item to position without CanAddToPosition check
	virtual bool SetContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position);

	virtual void HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2f, UContainerItemBase*> ItemsMap;

	// Setup 0 to infinity 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxRows = 0;

	// Setup 0 to infinity 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCollumns = 0;

	//Horizontal - Column by column
	//Vertical - Row by row
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridContainerDirection FillDirection = EGridContainerDirection::Vertical;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemAdded OnGridContainerItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemRemoved OnGridContainerItemRemoved;
};
