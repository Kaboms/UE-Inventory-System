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
		return nullptr;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2f Size;
	
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
	FVector2f Position;

	UPROPERTY(BlueprintReadWrite)
	UTetrisContainerSlotMetadata* SlotMetadata;

	UPROPERTY(BlueprintReadWrite)
	UContainerItemBase* ContainerItem;
};

/**
 * Tetris-like inventory system
 */
UCLASS()
class INVENTORYSYSTEM_API UTetrisItemsContainer : public UGridItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual void InitDefaultItems() override;

	virtual bool AddContainerItems(TArray<UContainerItemBase*> ContainerItems) override;

	virtual bool AddContainerItem(UContainerItemBase* ContainerItem) override;

	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	virtual  bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2f& OutPos) override;

	UFUNCTION(BlueprintCallable)
	bool CanPlaceTo(UTetrisContainerSlotMetadata* SlotMetadata, FVector2f SelectedSlotCell, FVector2f TargetSlotCell, UPARAM(Ref) TArray<FVector2f>& OutOccupiedCells) const;

	UFUNCTION(BlueprintCallable)
	void PlaceToCell(UContainerItemBase* ContainerItem, FVector2f SelectedSlotCell, FVector2f TargetSlotCell);

	UFUNCTION(BlueprintCallable)
	static void SortItemsBySlotSize(TArray<UContainerItemBase*>& ItemsToSort);

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2f, FTetrisContainerSlot> ItemsMap;

private:
	// For fast access to item slots
	TMap<UContainerItemBase*, FTetrisContainerSlot> ItemsSlots;
};
