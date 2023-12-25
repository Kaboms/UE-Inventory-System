#pragma once

#include "CoreMinimal.h"

#include "ContainerItemBase.generated.h"

class UItemBase;
class UItemData;
class UItemsContainerBase;

USTRUCT(BlueprintType)
struct FDefaultContainerItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UContainerItemBase* ContainerItem;

	UPROPERTY(EditAnywhere)
	UItemData* Item;
};

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UContainerItemBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void AddItem(UItemBase* InItem, int32 InAmount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void SetItem(UItemBase* NewItem, int32 InAmount = 1);

	UFUNCTION(BlueprintCallable)
	virtual void Drop(int32 AmountToDrop = 1);

	UFUNCTION(BlueprintCallable)
	virtual void DropAll();

	UFUNCTION(BlueprintCallable)
	virtual void Remove(int32 AmountToRemove = 1);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveAll();

	UFUNCTION(BlueprintCallable)
	bool MergeWithOther(UContainerItemBase* OtherContainerItem);

	UFUNCTION(BlueprintCallable)
	UContainerItemBase* Split(int32 SplitAmount);

	UFUNCTION(BlueprintGetter)
	int32 GetAmount();

	UFUNCTION(BlueprintSetter)
	void SetAmount(int32 NewAmount);

	UFUNCTION(BlueprintGetter)
	UItemBase* GetItem();

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

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetAmount, BlueprintSetter = SetAmount)
	int32 Amount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter = GetItem, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UItemBase> Item;
};