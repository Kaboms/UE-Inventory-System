#pragma once

#include "CoreMinimal.h"

#include "Inventory/Containers/ContainerItemBase.h"

#include "GridContainerItem.generated.h"

class UGridItemsContainer;

UCLASS()
class INVENTORYSYSTEM_API UGridContainerItem : public UContainerItemBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UGridContainerItem* ConstructFromContainerItem(UGridItemsContainer* InContainer, UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintPure)
	UGridItemsContainer* GetGridContainer() { return Cast<UGridItemsContainer>(Container); }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Position;
};