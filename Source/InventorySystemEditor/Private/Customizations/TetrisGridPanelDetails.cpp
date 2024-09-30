// Copyright Epic Games, Inc. All Rights Reserved.

#include "Customizations/TetrisGridPanelDetails.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "UMG/TetrisGridPanel.h"
#include "PropertyCustomizationHelpers.h"

TSharedRef<IDetailCustomization> FTetrisGridPanelDetails::MakeInstance()
{
	return MakeShareable(new FTetrisGridPanelDetails());
}

void FTetrisGridPanelDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailLayout.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() != 1)
	{
		return;
	}
	UTetrisGridPanel* TetrisGridPanel = Cast<UTetrisGridPanel>(Objects[0].Get());
	if (!TetrisGridPanel)
	{
		return;
	}

	IDetailCategoryBuilder& SlotsCategory = DetailLayout.EditCategory(TEXT("Slots"));
	AddSlotClassPicker(*TetrisGridPanel, SlotsCategory, DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UTetrisGridPanel, SlotWidgetClass)));
}

const UClass* FTetrisGridPanelDetails::GetSelectedSlotClass() const
{
	if (SlotClassHandle.IsValid())
	{
		const UObject* SelectedClassObj = nullptr;
		SlotClassHandle->GetValue(SelectedClassObj);
		return Cast<UClass>(SelectedClassObj);

	}
	return nullptr;
}

void FTetrisGridPanelDetails::HandleNewSlotClassSelected(const UClass* NewSlotClass) const
{
	if (SlotClassHandle.IsValid())
	{
		SlotClassHandle->SetValueFromFormattedString(NewSlotClass->GetPathName());
	}
}

void FTetrisGridPanelDetails::AddSlotClassPickerInternal(const UClass* SlotBaseClass, const UClass* RequiredSlotInterface, IDetailCategoryBuilder& CategoryBuilder) const
{
	// Create a custom class picker here that filters according to the SlotClass 
	IDetailPropertyRow& SlotClassRow = CategoryBuilder.AddProperty(SlotClassHandle);
	TSharedPtr<SWidget> NameWidget;
	TSharedPtr<SWidget> ValueWidget;
	FDetailWidgetRow Row;
	SlotClassRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);

	SlotClassRow.CustomWidget()
		.NameContent()
		[
			NameWidget.ToSharedRef()
		]
		.ValueContent()
		.MinDesiredWidth(Row.ValueWidget.MinWidth)
		.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
		[
			SNew(SClassPropertyEntryBox)
				.AllowNone(false)
				.IsBlueprintBaseOnly(true)
				.RequiredInterface(RequiredSlotInterface)
				.MetaClass(SlotBaseClass ? SlotBaseClass : UUserWidget::StaticClass())
				.SelectedClass(this, &FTetrisGridPanelDetails::GetSelectedSlotClass)
				.OnSetClass(this, &FTetrisGridPanelDetails::HandleNewSlotClassSelected)
		];
}
