#pragma once

#include "CoreMinimal.h"

#include "UseableInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UUseableInterface : public UInterface
{
	GENERATED_BODY()
};

class IUseableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use();
};