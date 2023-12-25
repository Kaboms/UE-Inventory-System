#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventoryComponentBase.generated.h"

class UContainerItemBase;
class UEquipSlot;
class UItemBase;
class UItemsContainerBase;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventoryComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintGetter)
	UItemsContainerBase* GetItemsContainer() const { return ItemsContainer; }

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetItemsContainer)
	TObjectPtr<UItemsContainerBase> ItemsContainer;
};