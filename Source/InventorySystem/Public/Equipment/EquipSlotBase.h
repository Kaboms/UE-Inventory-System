#pragma once

#include "CoreMinimal.h"

#include "Interfaces/EquipableInterface.h"
#include "GameplayTagContainer.h"
#include "EquipSlotBase.generated.h"

class UContainerItemBase;
class UItemData;
class UEquipmentContainer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquiped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeOff);

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UEquipSlotBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool Equip(UObject* Instigator, UContainerItemBase* ContainerItemToEquip);

	UFUNCTION(BlueprintPure)
	bool CanEquip(UItemData* ItemData);

	UFUNCTION(BlueprintPure)
	bool HasItem() { return IsValid(ContainerItem); }

	UFUNCTION(BlueprintPure)
	bool CanEquipContainerItem(UContainerItemBase* ContainerItemToEquip);

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION(BlueprintCallable)
	void Select();

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEquipmentContainer> EquipmentContainer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag RequiredItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UContainerItemBase> ContainerItem;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnEquiped OnEquiped;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnTakeOff OnTakeOff;
};