// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "Blueprint/UserWidget.h"

class IDetailCategoryBuilder;
class IPropertyHandle;

class FTetrisGridPanelDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

protected:
	template <typename WidgetT>
	void AddSlotClassPicker(WidgetT& WidgetInstance, IDetailCategoryBuilder& CategoryBuilder, TSharedRef<IPropertyHandle> InSlotClassHandle)
	{
		static const FName SlotInterfaceMetaKey = TEXT("SlotInterface");
		static const FName SlotClassMetaKey = TEXT("SlotClass");

		SlotClassHandle = InSlotClassHandle;

		const UClass* RequiredInterface = nullptr;
		const UClass* BaseClass = nullptr;

		if (UUserWidget* OwningUserWidget = WidgetInstance.template GetTypedOuter<UUserWidget>())
		{
			// Find the native BindWidget FProperty that corresponds to this table view
			for (TFieldIterator<FObjectProperty> PropertyIter(OwningUserWidget->GetClass()); PropertyIter; ++PropertyIter)
			{
				FObjectProperty* Property = *PropertyIter;
				if (Property->PropertyClass && Property->PropertyClass->IsChildOf<WidgetT>())
				{
					WidgetT** WidgetPropertyInstance = Property->ContainerPtrToValuePtr<WidgetT*>(OwningUserWidget);
					if (*WidgetPropertyInstance == &WidgetInstance)
					{
						BaseClass = Property->GetClassMetaData(SlotClassMetaKey);
						RequiredInterface = Property->GetClassMetaData(SlotInterfaceMetaKey);
						break;
					}
				}
			}
		}

		// If the property binding didn't exist or didn't specify a class/interface, check with the class itself (including parents as needed)
		const UClass* WidgetClass = WidgetInstance.GetClass();
		while (WidgetClass && WidgetClass->IsChildOf<UWidget>() && (!BaseClass || !RequiredInterface))
		{
			if (!BaseClass)
			{
				BaseClass = WidgetClass->GetClassMetaData(SlotClassMetaKey);
			}
			if (!RequiredInterface)
			{
				RequiredInterface = WidgetClass->GetClassMetaData(SlotInterfaceMetaKey);
			}
			WidgetClass = WidgetClass->GetSuperClass();
		}

		// If a valid base class has been specified, create a custom class picker that filters accordingly
		if (BaseClass || RequiredInterface)
		{
			AddSlotClassPickerInternal(BaseClass, RequiredInterface, CategoryBuilder);
		}
	}

private:
	void AddSlotClassPickerInternal(const UClass* SlotClass, const UClass* RequiredSlotInterface, IDetailCategoryBuilder& CategoryBuilder) const;

	const UClass* GetSelectedSlotClass() const;
	void HandleNewSlotClassSelected(const UClass* NewSlotClass) const;

	TSharedPtr<IPropertyHandle> SlotClassHandle;
};
