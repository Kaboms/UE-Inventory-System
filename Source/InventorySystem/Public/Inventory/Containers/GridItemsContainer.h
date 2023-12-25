#pragma once

#include "CoreMinimal.h"
#include "Inventory/Containers/ItemsContainerBase.h"
#include "GridItemsContainer.generated.h"

UENUM(BlueprintType)
enum class EGridContainerDirection : uint8
{
	Horizontal,
	Vertical
};

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UGridItemsContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual bool AddItem(UItemBase* Item) override;

	virtual bool AddItems(TArray<UItemBase*> Items) override;

	virtual void AddContainerItem(UContainerItemBase* ContainerItem) override;

	virtual void RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	UFUNCTION(BlueprintCallable)
	bool FindContainerItemPosition(UContainerItemBase* ContainerItem, FVector2D& OutPos);

	// Refiil grid to remove empty spaces
	UFUNCTION(BlueprintCallable)
	void Refill();

	UFUNCTION(BlueprintCallable)
	bool MoveToDirection(FVector2D& CurrentPosition, EGridContainerDirection Direction);

	UFUNCTION(BlueprintCallable)
	bool MoveItemToPosition(FVector2D& ItemPosition, FVector2D NewItemPosition);

	bool IsPositionValid(FVector2D Position);

protected:
	bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2D& Position);

public:
	// Setup 0 to infinity 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxRows = 0;

	// Setup 0 to infinity 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxCollumns = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridContainerDirection FillDirection = EGridContainerDirection::Vertical;

	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2D, UContainerItemBase*> ItemsMap;
};
