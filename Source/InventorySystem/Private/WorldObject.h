#pragma once

#include "UObject/Object.h"
#include "WorldObject.generated.h"

class UWorld;

UCLASS(Abstract, BlueprintType)
class UWorldObject : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override
	{
		if (GetOuter() == nullptr)
		{
			return nullptr;
		}

		// Special case for behavior tree nodes in the editor
		if (Cast<UPackage>(GetOuter()) != nullptr)
		{
			// GetOuter should return a UPackage and its Outer is a UWorld
			return Cast<UWorld>(GetOuter()->GetOuter());
		}

		// In all other cases...
		return GetOuter()->GetWorld();
	}
};