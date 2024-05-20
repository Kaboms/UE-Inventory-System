// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"


class FInventorySystemEditorStyle
    : public FSlateStyleSet
{
public:
	static FInventorySystemEditorStyle& Get();

protected:
	friend class FInventorySystemEditorModule;

	static void Register();
	static void Unregister();

private:
	FInventorySystemEditorStyle();
};
