// Copyright Epic Games, Inc. All Rights Reserved.

#include "UMG/TetrisGridPanel.h"
#include "UMG/TetrisGridSlot.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Slate/STetrisGridPanel.h"
#include "Editor/WidgetCompilerLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetrisGridPanel)

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UTetrisGridPanel

UTetrisGridPanel::UTetrisGridPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UTetrisGridPanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyTetrisGridPanel.Reset();
}

UClass* UTetrisGridPanel::GetSlotClass() const
{
	return UTetrisGridSlot::StaticClass();
}

void UTetrisGridPanel::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if (MyTetrisGridPanel.IsValid())
	{
		CastChecked<UTetrisGridSlot>(InSlot)->BuildSlot(MyTetrisGridPanel.ToSharedRef());
	}
}

void UTetrisGridPanel::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyTetrisGridPanel.IsValid() && InSlot->Content)
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if (Widget.IsValid())
		{
			MyTetrisGridPanel->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UTetrisGridPanel::RebuildWidget()
{
	MyTetrisGridPanel = SNew(STetrisGridPanel);

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UTetrisGridSlot* TypedSlot = Cast<UTetrisGridSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyTetrisGridPanel.ToSharedRef());
		}
	}

	return MyTetrisGridPanel.ToSharedRef();
}

UTetrisGridSlot* UTetrisGridPanel::AddChildToGrid(UWidget* Content, int32 InRow, int32 InColumn)
{
	UTetrisGridSlot* GridSlot = Cast<UTetrisGridSlot>(Super::AddChild(Content));

	if (GridSlot != nullptr)
	{
		GridSlot->SetRow(InRow);
		GridSlot->SetColumn(InColumn);
	}

	return GridSlot;
}

float UTetrisGridPanel::GetCellSize() const
{
	return CellSize;
}

void UTetrisGridPanel::SetCellSize(float InCellSize)
{
	CellSize = InCellSize;

	if (MyTetrisGridPanel)
	{
		MyTetrisGridPanel->SetCellSize(CellSize);
	}
}

void UTetrisGridPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

#if WITH_EDITOR

const FText UTetrisGridPanel::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

