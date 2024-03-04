#pragma once

#include "CoreMinimal.h"

#include "Interfaces/EquipableInterface.h"
#include "GameplayTagContainer.h"
#include "EquipSlotBase.generated.h"

class UContainerItemBase;
class UItemData;

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UEquipSlotBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Equip(UContainerItemBase* ContainerItemToEquip);

	UFUNCTION(BlueprintPure)
	bool CanEquip(UItemData* ItemData);

	UFUNCTION(BlueprintPure)
	bool CanEquipContainerItem(UContainerItemBase* ContainerItemToEquip);

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION(BlueprintCallable)
	void Select();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag RequiredItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UContainerItemBase> ContainerItem;
};