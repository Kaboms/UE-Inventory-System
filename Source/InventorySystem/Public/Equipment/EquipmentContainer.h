#pragma once

#include "CoreMinimal.h"
#include "Inventory/Containers/ItemsContainerBase.h"
#include "EquipmentContainer.generated.h"

class UEquipSlotBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipedToSlot, FGameplayTag, SlotTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeOffFromSlot, FGameplayTag, SlotTag);

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UEquipmentContainer : public UItemsContainerBase
{
	GENERATED_BODY()

public:
	void Init();

	virtual bool AddItem(UItemBase* Item) override;

	virtual void AddContainerItem(UContainerItemBase* ContainerItem);

	virtual void RemoveContainerItem(UContainerItemBase* ContainerItem);

	virtual TArray<UContainerItemBase*> GetContainerItems() override;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UEquipSlotBase*> InstancedSlots;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UEquipSlotBase*> Slots;

	UPROPERTY(BlueprintReadWrite)
	UObject* Owner;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnEquipedToSlot OnEquipedToSlot;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnTakeOffFromSlot OnTakeOffFromSlot;
};
