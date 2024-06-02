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
	UFUNCTION(BlueprintCallable)
	virtual  bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos);

	// Refiil grid to remove empty spaces
	UFUNCTION(BlueprintCallable)
	virtual void Refill();

	virtual bool Move(float& XPosition, const int32& XMaxPosition, float& YPosition, const int32& YMaxPosition);

	// Swap items within same container.
	UFUNCTION(BlueprintCallable)
	virtual void SwapItemsPositions(FVector2f A, FVector2f B);

	// Is Position >= 0 and < MaxRows and MaxCollumns
	UFUNCTION(BlueprintPure)
	bool IsPositionValid(FVector2f Position);

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

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemAdded OnGridContainerItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnGridContainerItemRemoved OnGridContainerItemRemoved;
};
