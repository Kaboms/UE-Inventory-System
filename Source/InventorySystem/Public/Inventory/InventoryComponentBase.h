#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Inventory/Containers/ItemsContainerBase.h"

#include "InventoryComponentBase.generated.h"

class UContainerItemBase;
class UEquipSlot;
class UItemBase;
class UItemsContainerBase;

// All inventory login contained in UItemsContainerBase classes.
// Container items can take the access to actor from this component.
// This component may have several item containers.
UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintGetter)
	UItemsContainerBase* GetItemsContainer() const { return ItemsContainer; }

protected:
	virtual void OnContainerOpened();
	virtual void OnContainerClosed();

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetItemsContainer)
	TObjectPtr<UItemsContainerBase> ItemsContainer;

	UPROPERTY(BlueprintAssignable)
	FContainerOpened OnOpen;

	UPROPERTY(BlueprintAssignable)
	FContainerClosed OnClose;
};