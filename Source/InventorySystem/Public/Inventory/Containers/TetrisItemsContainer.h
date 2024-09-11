#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/GridItemsContainerBase.h"
#include "Inventory/ItemData.h"

#include "TetrisItemsContainer.generated.h"

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UTetrisContainerSlotMetadata : public UItemMetadata
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static UTetrisContainerSlotMetadata* FindMetadata(UItemData* ItemData)
	{
		if (UItemMetadata* ItemMetadata = ItemData->FindMetadataByClass(UTetrisContainerSlotMetadata::StaticClass()))
		{
			return Cast<UTetrisContainerSlotMetadata>(ItemMetadata);
		}
		else
		{
			UE_LOG(InventorySystem, Warning, TEXT("Failed to find TetrisMetadata from ItemData: %s"), *ItemData->GetPathName())
			return NewObject<UTetrisContainerSlotMetadata>(ItemData);
		}
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2f Size = FVector2f(1, 1);
	
	// Use this for exact slot form. By default slot is Rectangle setted by Size property
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FVector2f> Form;
};

USTRUCT(BlueprintType)
struct FTetrisContainerSlot
{
	GENERATED_BODY()

public:
	// Top Left position of item
	UPROPERTY(BlueprintReadWrite)
	FVector2f Position = FVector2f::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	UTetrisContainerSlotMetadata* SlotMetadata = nullptr;

	UPROPERTY(BlueprintReadWrite)
	UContainerItemBase* ContainerItem = nullptr;
};

/**
 * Tetris-like inventory system
 */
UCLASS()
class INVENTORYSYSTEM_API UTetrisItemsContainer : public UGridItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual  bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos) override;

	UFUNCTION(BlueprintCallable)
	bool CanPlaceTo(UTetrisContainerSlotMetadata* SlotMetadata, FVector2f SelectedSlotCell, FVector2f TargetSlotCell, UPARAM(Ref) TArray<FVector2f>& OutOccupiedCells) const;

	// Place to cell without any checks.
	UFUNCTION(BlueprintCallable)
	void PlaceToCell(UContainerItemBase* ContainerItem, FVector2f SelectedSlotCell, FVector2f TargetSlotCell);

	UFUNCTION(BlueprintCallable)
	bool GetSlotFromPosition(FVector2f Position, FTetrisContainerSlot& OutSlot);

	UFUNCTION(BlueprintCallable)
	static void SortItemsBySlotSize(TArray<UContainerItemBase*>& ItemsToSort);

	virtual bool CanAddToPosition(UContainerItemBase* ContainerItem, FVector2f Position) const override;

	virtual void SwapItemsPositions(FVector2f A, FVector2f B) override;

protected:
	virtual void HandleAddContainerItemToPosition(UContainerItemBase* ContainerItem, const FVector2f& Position);

	virtual void HandleContainerItemRemoved(UContainerItemBase* ContainerItem) override;

	virtual void EmptyPosition(const FVector2f& Position);

private:
	// For fast access to item slots
	TMap<UContainerItemBase*, FTetrisContainerSlot> ItemsSlots;
};
