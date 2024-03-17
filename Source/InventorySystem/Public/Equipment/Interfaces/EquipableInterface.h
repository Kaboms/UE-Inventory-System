#pragma once

#include "CoreMinimal.h"
#include "EquipableInterface.generated.h"

// Implment for any item that may be equiped
UINTERFACE(MinimalAPI, Blueprintable)
class UEquipableInterface : public UInterface
{
	GENERATED_BODY()
};

class IEquipableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Select();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Equip(UObject* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeOff();
};