#pragma once

#include "CoreMinimal.h"

#include "Interfaces/EquipableInterface.h"
#include "GameplayTagContainer.h"
#include "EquipSlotBase.generated.h"

class UItemBase;

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UEquipSlotBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Equip(UItemBase* ItemToEquip);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag RequiredItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemBase> Item;
};