// Copyright Epic Games, Inc. All Rights Reserved.

#include "UMG/TetrisGridSlot.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UTetrisGridSlot

UTetrisGridSlot::UTetrisGridSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Slot(nullptr)
{
	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;

	SlotSize = FVector2D(1, 1);
}

void UTetrisGridSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = nullptr;
}

void UTetrisGridSlot::BuildSlot(TSharedRef<STetrisGridPanel> TetrisGridPanel)
{
	TetrisGridPanel->AddSlot(Column, Row, SlotSize, SlotForm, STetrisGridPanel::Layer(STetrisGridPanel::Layer::ContentLayer))
		.Expose(Slot)
		.Padding(Padding)
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		[
			Content == nullptr ? SNullWidget::NullWidget : Content->TakeWidget()
		];
}

FMargin UTetrisGridSlot::GetPadding() const
{
	return Slot ? Slot->GetPadding() : Padding;
}

void UTetrisGridSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if (Slot)
	{
		Slot->SetPadding(InPadding);
	}
}

int32 UTetrisGridSlot::GetRow() const
{
	return Slot ? Slot->GetRow() : Row;
}

void UTetrisGridSlot::SetRow(int32 InRow)
{
	Row = InRow;
	if (Slot)
	{
		Slot->SetRow(InRow);
	}
}

int32 UTetrisGridSlot::GetColumn() const
{
	return Slot ? Slot->GetColumn() : Column;
}


void UTetrisGridSlot::SetColumn(int32 InColumn)
{
	Column = InColumn;
	if (Slot)
	{
		Slot->SetColumn(InColumn);
	}
}

void UTetrisGridSlot::SetSlotSize(FVector2D InSlotSize)
{
	if (InSlotSize.X <= 0 || InSlotSize.Y <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid slot size X: %d; Y: %d"), InSlotSize.X, InSlotSize.Y);
	}

	SlotSize = InSlotSize;
	if (Slot)
	{
		Slot->SetSlotSize(SlotSize);
	}
}

FVector2D UTetrisGridSlot::GetSlotSize() const
{
	return SlotSize;
}

void UTetrisGridSlot::SetSlotForm(TSet<FVector2D> InSlotForm)
{
	SlotForm = InSlotForm;
	if (Slot)
	{
		Slot->SetForm(SlotForm);
	}
}

TSet<FVector2D> UTetrisGridSlot::GetSlotForm() const
{
	return SlotForm;
}

EHorizontalAlignment UTetrisGridSlot::GetHorizontalAlignment() const
{
	return Slot ? Slot->GetHorizontalAlignment() : HorizontalAlignment.GetValue();
}

void UTetrisGridSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if (Slot)
	{
		Slot->SetHorizontalAlignment(InHorizontalAlignment);
	}
}

EVerticalAlignment UTetrisGridSlot::GetVerticalAlignment() const
{
	return Slot ? Slot->GetVerticalAlignment() : VerticalAlignment.GetValue();
}

void UTetrisGridSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if (Slot)
	{
		Slot->SetVerticalAlignment(InVerticalAlignment);
	}
}

void UTetrisGridSlot::SynchronizeProperties()
{
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
	SetPadding(Padding);

	SetRow(Row);
	SetColumn(Column);

	SetSlotSize(SlotSize);
}

#if WITH_EDITOR
bool UTetrisGridSlot::NudgeByDesigner(const FVector2D& NudgeDirection, const TOptional<int32>& GridSnapSize)
{
	const FVector2D ClampedDirection = NudgeDirection.ClampAxes(-1.0f, 1.0f);
	const int32 NewColumn = GetColumn() + ClampedDirection.X;
	const int32 NewRow = GetRow() + ClampedDirection.Y;

	if (NewColumn < 0 || NewRow < 0 || (NewColumn == GetColumn() && NewRow == GetRow()))
	{
		return false;
	}

	Modify();

	SetRow(NewRow);
	SetColumn(NewColumn);

	return true;
}

void UTetrisGridSlot::SynchronizeFromTemplate(const UPanelSlot* const TemplateSlot)
{
	const ThisClass* const TemplateTetrisGridSlot = CastChecked<ThisClass>(TemplateSlot);
	SetRow(TemplateTetrisGridSlot->GetRow());
	SetColumn(TemplateTetrisGridSlot->GetColumn());
}
#endif

UTetrisGridSlot* UTetrisGridSlot::SlotAsTetrisGridSlot(UWidget* Widget)
{
	if (Widget)
	{
		return Cast<UTetrisGridSlot>(Widget->Slot);
	}

	return nullptr;
}
