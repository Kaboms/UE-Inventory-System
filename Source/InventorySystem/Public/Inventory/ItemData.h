#pragma once

#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemData.generated.h"

class UItemBase;

UCLASS(Blueprintable)
class INVENTORYSYSTEM_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UItemData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemPickupClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemBase> ItemInstanceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int StackSize = 1;
};
