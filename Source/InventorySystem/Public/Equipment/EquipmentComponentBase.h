#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "EquipmentComponentBase.generated.h"

class UEquipmentContainer;

UCLASS(BlueprintType, Blueprintable, Meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UEquipmentComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintGetter)
	UEquipmentContainer* GetEquipmentContainer() const { return EquipmentContainer; }

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetEquipmentContainer)
	TObjectPtr<UEquipmentContainer> EquipmentContainer;
};