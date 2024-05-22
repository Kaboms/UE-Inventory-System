#pragma once

#include "CoreMinimal.h"

#include "UsableInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class UUsableInterface : public UInterface
{
	GENERATED_BODY()
};

class IUsableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Use(UObject* Instigator);

	// "Use", "Read", "Break" etc.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetActionDisplayName();
};