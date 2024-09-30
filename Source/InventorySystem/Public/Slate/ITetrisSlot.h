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
};