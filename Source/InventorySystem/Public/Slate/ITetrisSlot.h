#pragma once

#include "UObject/Interface.h"
#include "ITetrisSlot.generated.h"

class UUserWidget;

UINTERFACE(BlueprintType)
class INVENTORYSYSTEM_API UTetrisSlot : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class INVENTORYSYSTEM_API ITetrisSlot : public IInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnSlotHoverChanged(bool bIsHovered);

	UFUNCTION(BlueprintNativeEvent)
	void OnSlotUsedChanged(bool bIsSlotUsed);

	UFUNCTION(BlueprintNativeEvent)
	bool CanUseSlot();

};