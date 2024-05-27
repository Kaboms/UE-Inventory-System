// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemDataFactory.h"
#include "Inventory/ItemData.h"

UItemDataFactory::UItemDataFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UItemData::StaticClass();
}

UObject* UItemDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UItemData* NewItemData = NewObject<UItemData>(InParent, Class, Name, Flags | RF_Transactional);
	return NewItemData;
}
