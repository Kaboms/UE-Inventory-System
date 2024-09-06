#pragma once

#include "CoreMinimal.h"
#include "WorldObject.h"

#include "ItemBase.generated.h"

class UItemData;

// Inherit from this class and implement login and behavioud of item.
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemBase : public UWorldObject
{
	GENERATED_BODY()

public:
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	virtual bool IsDroppable();

	UFUNCTION(BlueprintCallable)
	static bool DoesItemClassImplementInterface(TSubclassOf<UItemBase> ItemClass, TSubclassOf<UInterface> Interface);

	UFUNCTION(BlueprintPure)
	virtual bool CanMergeWithItem(UItemBase* OtherItem);

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "IsDroppable"))
	bool ReceiveIsDroppable();

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Init"))
	void ReceiveInit();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	UItemData* ItemData;
};

