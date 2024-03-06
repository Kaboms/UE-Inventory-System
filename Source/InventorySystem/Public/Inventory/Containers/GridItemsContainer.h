#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/ItemsContainerBase.h"

#include "GridItemsContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContainerItemAdded, FVector2f, ItemPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContainerItemRemoved, FVector2f, ItemPos);

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

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemToPosition(UContainerItemBase* ContainerItem, FVector2f Position);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveContainerItemFromPosition(FVector2f Position);

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position);


	virtual void RemoveContainerItem(UContainerItemBase* ContainerItem) override;

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

	// Return false if items merge is failed
	UFUNCTION(BlueprintCallable)
	bool MoveItem(UContainerItemBase* ContainerItem, FVector2f NewItemPosition);

	UFUNCTION(BlueprintCallable)
	bool MoveItemByPosition(FVector2f ItemPosition, FVector2f NewItemPosition);

	// Swap items within same container.
	UFUNCTION(BlueprintCallable)
	void SwapItemsPositions(FVector2f A, FVector2f B);

	// Swap items within same container.
	UFUNCTION(BlueprintCallable)
	void SwapItems(UContainerItemBase* A, UContainerItemBase* B);

	bool IsPositionValid(FVector2f Position);

protected:
	bool AddContainerItemFromPosition(UItemBase* Item, FVector2f& Position);
	bool FindFreePosition(UItemBase* Item, FVector2f& Position);

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
	TMap<FVector2f, UContainerItemBase*> ItemsMap;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnContainerItemAdded OnContainerItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnContainerItemRemoved OnContainerItemRemoved;
};
