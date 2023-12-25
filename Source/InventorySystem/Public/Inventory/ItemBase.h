#pragma once

#include "CoreMinimal.h"
#include "WorldObject.h"

#include "ItemBase.generated.h"

class UItemData;

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UItemBase : public UWorldObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual bool IsDroppable();

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "IsDroppable"))
	bool ReceiveIsDroppable();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	UItemData* ItemData;
};

