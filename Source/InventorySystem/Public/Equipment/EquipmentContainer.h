#pragma once

#include "CoreMinimal.h"
#include "Inventory/Containers/ItemsContainerBase.h"
#include "EquipmentContainer.generated.h"

class UEquipSlotBase;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UEquipmentContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	UEquipmentContainer();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, UEquipSlotBase*> Slots;
};
