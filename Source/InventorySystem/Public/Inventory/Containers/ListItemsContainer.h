#pragma once

#include "CoreMinimal.h"
#include "Inventory/Containers/ItemsContainerBase.h"
#include "ListItemsContainer.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UListItemsContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	virtual bool AddContainerItem(UContainerItemBase* ContainerItem) override;

	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem) override;

	virtual TArray<UContainerItemBase*> GetContainerItems() override { return Items; }

public:
	// Set to -1 to infinity
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxItems = -1;

	UPROPERTY(BlueprintReadOnly)
	TArray<UContainerItemBase*> Items;
};
