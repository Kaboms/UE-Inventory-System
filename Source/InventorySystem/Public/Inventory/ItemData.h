#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemData.generated.h"

class UItemBase;

UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemMetadata : public UObject
{
	GENERATED_BODY()
};

// Base item data class. Must contains only static data that setup from editor (eg. MaxDurability)
// To apply dynamic data to you item (eg. CurrentDurability) set it to UItemBase instance.
UCLASS(Blueprintable)
class INVENTORYSYSTEM_API UItemData : public UDataAsset
{
	GENERATED_BODY()

public:
	// Construct item instance from ItemInstanceClass
	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Outer"))
	static UItemBase* ConstructItemInstance(UObject* Outer, UItemData* ItemData);

public:
	UItemData();

	// Construct item instance from ItemInstanceClass
	UItemBase* ConstructItemInstance(UObject* Outer);

	UItemMetadata* FindMetadataByClass(TSubclassOf<UItemMetadata> MetadataClass);

public:
	// Unique ID for your Item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText Description;

	// Item type. Can be used for sorting in containers or by filtering acceptable item type for equip slot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTagContainer Type;

	// You can spawn this actor when drop item from inventory.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ItemPickupClass;

	// Actor instance of the class.
	// Each of this actor must hame own ItemInstanceClass instance.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ItemActorClass;

	// Item instance class for current data.
	// Assume you have SwordItemData with some weapon params (Damage, Durability).
	// So it's good idea to create the USwordItem and implement logic and behaviour for sword in it.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Slot")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Slot")
	int32 StackSize = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TArray<UItemMetadata*> MetadataEntries;

	inline bool operator== (const UItemData* Other)
	{
		return ID == Other->ID;
	}
};
