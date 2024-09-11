#pragma once

#include "InventorySystem.h"

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "WorldObject.h"
#include "Inventory/Containers/ContainerItemBase.h"

#include "ItemsContainerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FContainerOpened);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FContainerClosed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContainerItemAdded, class UContainerItemBase*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContainerItemRemoved, class UContainerItemBase*, Item);

class UItemBase;
class UItemData;

// Abstract class for any containers in the game.
// Idea to have own class for containers implemented from UObject instead of implement all login in some UActorComponent is very simple.
// If implement all logic in ActorComponent we cannot have nested containers (container with containers)
UCLASS(Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemsContainerBase : public UWorldObject
{
	GENERATED_BODY()

public:
	// Create ContainerItem with Item and add it to Container
	// Return True on success addition.
	// Return false if here is no enought space or something like this
	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	bool AddItems(TArray<UItemBase*> Items);

	// Create ContainerItem with ItemData and add it to Container
	// UItemBase instance will not create in this case.
	UFUNCTION(BlueprintCallable)
	bool AddItemData(UItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	bool AddItemsData(TArray<UItemData*> ItemsData);

	UFUNCTION(BlueprintCallable)
	virtual void InitDefaultItems();

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintCallable)
	virtual bool AddContainerItems(TArray<UContainerItemBase*> ContainerItems);

	UFUNCTION(BlueprintCallable)
	virtual UContainerItemBase* CreateContainerItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable)
	virtual UContainerItemBase* CreateContainerItemFromData(UItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	virtual UContainerItemBase* FindContainerItem(UItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	virtual TArray<UContainerItemBase*> GetContainerItems();

	UFUNCTION(BlueprintCallable)
	virtual void Open();

	UFUNCTION(BlueprintCallable)
	virtual void Close();

	UFUNCTION(BlueprintPure)
	virtual bool CanSwapItems(UContainerItemBase* ContainerItem, UContainerItemBase* OtherItem);

	UFUNCTION(BlueprintCallable)
	bool MergeItem(UContainerItemBase* ContainerItem, UContainerItemBase* OtherItem);

	UFUNCTION(BlueprintPure)
	bool IsEmpty();

	UFUNCTION(BlueprintPure)
	bool IsNotEmpty() { return !IsEmpty(); };

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Add Container Item"))
	bool ReceiveAddContainerItem(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Remove Container Item"))
	bool ReceiveRemoveContainerItem(UContainerItemBase* ContainerItem);

	virtual void HandleContainerItemRemoved(UContainerItemBase* ContainerItem);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Get Container Items"))
	TArray<UContainerItemBase*> ReceiveGetContainerItems();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Open")
	void ReceiveOpen();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Close")
	void ReceiveClose();

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<UContainerItemBase*> DefaultContainerItems;

public:
	UPROPERTY(BlueprintAssignable)
	FContainerOpened OnOpen;

	UPROPERTY(BlueprintAssignable)
	FContainerClosed OnClose;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnContainerItemAdded OnContainerItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FOnContainerItemRemoved OnContainerItemRemoved;
};