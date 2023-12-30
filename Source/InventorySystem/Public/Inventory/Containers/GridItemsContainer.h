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

	UFUNCTION(BlueprintCallable)
	bool MoveItemToPosition(FVector2f& ItemPosition, FVector2f NewItemPosition);

	bool IsPositionValid(FVector2f Position);

protected:
	bool AddContainerItemFromPosition(UContainerItemBase* ContainerItem, FVector2f& Position);

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
};
