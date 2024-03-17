#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/ItemsContainerBase.h"

#include "GridItemsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridContainerItemAdded, FVector2f, ItemPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGridContainerItemRemoved, FVector2f, ItemPos);

UENUM(BlueprintType)
enum class EGridContainerDirection : uint8
{
	Horizontal, // Column by column
	Vertical // Row by row
};

/**
 * Grid containers with free or fixes items positions.
 */
UCLASS()
class INVENTORYSYSTEM_API UGridItemsContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual bool AddContainerItems(TArray<UContainerItemBase*> ContainerItems) override;

	virtual bool AddContainerItem(UContainerItemBase* ContainerItem) override;

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveContainerItemFromPosition(FVector2f Position);

	// Find free position starded from Position and ContainerItem to it.
	// Return false if here is no free position to place item.
	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position);

	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	virtual void InitDefaultItems() override;

	virtual TArray<UContainerItemBase*> GetContainerItems() override;

	UFUNCTION(BlueprintCallable)
	bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos);

	// Refiil grid to remove empty spaces
	UFUNCTION(BlueprintCallable)
	void Refill();

	UFUNCTION(BlueprintCallable)
	bool MoveToFillDirection(FVector2f& CurrentPosition);

	bool Move(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition);

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition);

	// Return true if item moved to new position
	UFUNCTION(BlueprintCallable)
	bool MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition);

	// Swap items within same container.
	UFUNCTION(BlueprintCallable)
	void SwapItemsPositions(FVector2f A, FVector2f B);

	// Is Position >= 0 and < MaxRows and MaxCollumns
	UFUNCTION(BlueprintPure)
	bool IsPositionValid(FVector2f Position);

protected:
	bool FindFreePosition(UContainerItemBase* ContainerItem, FVector2f& Position);

public:
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

	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2f, UContainerItemBase*> ItemsMap;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemAdded OnGridContainerItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemRemoved OnGridContainerItemRemoved;
};
