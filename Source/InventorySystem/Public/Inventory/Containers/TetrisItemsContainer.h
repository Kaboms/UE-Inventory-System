#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/ItemsContainerBase.h"
#include "Inventory/ItemData.h"

#include "TetrisItemsContainer.generated.h"

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UTetrisContainerSlotMetadata : public UItemMetadata
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2f Size;
	
	// Use this for exact slot form. By default slot is Rectangle setted by Size property
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FVector2f> Form;
};

/**
 * Tetris-like inventory system
 */
UCLASS()
class INVENTORYSYSTEM_API UTetrisItemsContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual void InitDefaultItems() override;

	UFUNCTION(BlueprintCallable)
	bool CanPlaceTo(UTetrisContainerSlotMetadata* SlotMetadata, FVector2f SelectedSlotCell, FVector2f TargetSlotCell, UPARAM(Ref) TArray<FVector2f>& OutOccupiedCells);

	UFUNCTION(BlueprintCallable)
	void PlaceToCell(UContainerItemBase* ContainerItem, FVector2f SelectedSlotCell, FVector2f TargetSlotCell);

protected:
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2f, UContainerItemBase*> ItemsMap;

	// Setup 0 to infinity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxRows = 0;

	// Setup 0 to infinity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCollumns = 0;
};
