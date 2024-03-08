#pragma once

#include "CoreMinimal.h"

#include "ContainerItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FContainerItemUpdated, UContainerItemBase*, ContainerItem);

class UItemBase;
class UItemData;
class UItemsContainerBase;

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UContainerItemBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Meta = (DefaultToSelf = "Outer", DeterminesOutputType = "ContainerItemClass"))
	static UContainerItemBase* ConstructContainerItem(UObject* Outer, UItemData* ItemData, TSubclassOf<UContainerItemBase> ContainerItemClass);

	UFUNCTION(BlueprintCallable)
	void Swap(UContainerItemBase* OtherContainerItem);

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(UItemBase* Item, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void AddItemData(UItemData* ItemData, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void SetItem(UItemBase* NewItem, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void SetItemData(UItemData* ItemData, int32 Amount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void Drop(int32 AmountToDrop = 1);

	UFUNCTION(BlueprintCallable)
	virtual void DropAll();

	UFUNCTION(BlueprintCallable)
	virtual void Remove(int32 AmountToRemove = 1);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveAll();

	UFUNCTION(BlueprintCallable)
	virtual void RemoveFromContainer();

	// Return true if here is no reminder. OtherContainerItem will remove from container If after merge it's amount is zero
	UFUNCTION(BlueprintCallable)
	bool MergeWithOther(UContainerItemBase* OtherContainerItem);

	UFUNCTION(BlueprintCallable)
	void MergeOrSwap(UContainerItemBase* OtherContainerItem);

	UFUNCTION(BlueprintCallable)
	UContainerItemBase* Split(int32 SplitAmount);

	UFUNCTION(BlueprintGetter)
	int32 GetAmount();

	UFUNCTION(BlueprintSetter)
	void SetAmount(int32 NewAmount);

	UFUNCTION(BlueprintGetter)
	UItemBase* GetItem();

	UFUNCTION(BlueprintGetter)
	UItemData* GetItemData();

	UFUNCTION(BlueprintPure)
	bool CanAddItem();

	UFUNCTION(BlueprintPure)
	bool IsItemSameType(UContainerItemBase* OtherItem);

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta=(DisplayName="Drop"))
	void ReceiveDrop(int32 AmountToDrop = 1);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Remove"))
	void ReceiveRemove(int32 AmountToRemove = 1);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "AddItem"))
	void ReceiveAddItem(UItemBase* InItem);

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UItemsContainerBase> Container;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FContainerItemUpdated OnContainerItemUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetAmount, BlueprintSetter = SetAmount)
	int32 Amount = 1;

	// If null - item instance will generate from item data
	UPROPERTY(EditAnywhere, BlueprintGetter = GetItem, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UItemBase> Item;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetItemData, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UItemData> ItemData;
};