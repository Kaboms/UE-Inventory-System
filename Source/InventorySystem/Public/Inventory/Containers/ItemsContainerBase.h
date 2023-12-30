#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "WorldObject.h"

#include "ItemsContainerBase.generated.h"

class UItemBase;
class UItemData;
class UContainerItemBase;
struct FDefaultContainerItem;

UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemsContainerBase : public UWorldObject
{
	GENERATED_BODY()

public:
	// Return True on success addition.
	// Return false if here is no enought space or something like this
	UFUNCTION(BlueprintCallable)
	virtual bool AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	virtual bool AddItems(TArray<UItemBase*> Items);

	UFUNCTION(BlueprintCallable)
	bool AddItemData(UItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	bool AddItemsData(TArray<UItemData*> ItemsData);

	UFUNCTION(BlueprintCallable)
	virtual void InitDefaultItems();

	UFUNCTION(BlueprintCallable)
	virtual void AddContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintCallable)
	virtual UContainerItemBase* CreateContainerItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	virtual UContainerItemBase* FindContainerItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	virtual TArray<UContainerItemBase*> GetContainerItems();

protected:
	UContainerItemBase* CreateItemFromDefault(const FDefaultContainerItem& DefaultContainerItem);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Add Item"))
	bool ReceiveAddItem(UItemBase* Item);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Add Container Item"))
	bool ReceiveAddContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Remove Container Item"))
	bool ReceiveRemoveContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Get Container Items"))
	TArray<UContainerItemBase*> ReceiveGetContainerItems();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FDefaultContainerItem> DefaultItems;
};