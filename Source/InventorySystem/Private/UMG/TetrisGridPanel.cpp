// Copyright Epic Games, Inc. All Rights Reserved.

#include "UMG/TetrisGridPanel.h"
#include "UMG/TetrisGridSlot.h"
#include "Slate/ITetrisSlot.h"
#include "Slate/SObjectWidget.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Editor/WidgetCompilerLog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetrisGridPanel)

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UTetrisGridPanel

UTetrisGridPanel::UTetrisGridPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EntryWidgetPool(*this)
{
	bIsVariable = false;
	SetVisibilityInternal(ESlateVisibility::SelfHitTestInvisible);
}

void UTetrisGridPanel::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	EntryWidgetPool.ReleaseAllSlateResources();
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
	MyTetrisGridPanel = SNew(STetrisGridPanel)
		.GridSize(GridSize)
		.OnGenerateTetrisSlot_UObject(this, &UTetrisGridPanel::HandleGenerateTetrisSlot);

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

TSharedRef<SWidget> UTetrisGridPanel::HandleGenerateTetrisSlot()
{
	return EntryWidgetPool.GetOrCreateInstance(SlotWidgetClass)->TakeWidget();
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

FVector2D UTetrisGridPanel::GetGridSize() const
{
	return GridSize;
}

void UTetrisGridPanel::SetGridSize(FVector2D InGridSize)
{
	GridSize = InGridSize;

	if (MyTetrisGridPanel)
	{
		MyTetrisGridPanel->SetGridSize(GridSize);
	}
}

void UTetrisGridPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyTetrisGridPanel)
	{
		MyTetrisGridPanel->SetGridSize(GridSize);
	}
}

#if WITH_EDITOR

const FText UTetrisGridPanel::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

void UTetrisGridPanel::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!SlotWidgetClass)
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_TetrisGridPanel_MissingSlotClass", "{0} has no SlotWidgetClass specified - required for any UTetrisGridPanel to function."), FText::FromString(GetName())));
	}
	else if (!SlotWidgetClass->ImplementsInterface(UTetrisSlot::StaticClass()))
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_TetrisGridPanel_SlotClassNotImplementingInterface", "'{0}' has SlotWidgetClass property set to'{1}' and that Class doesn't implement ITetrisSlotInterface - required for any UTetrisGridPanel to function."), FText::FromString(GetName()), FText::FromString(SlotWidgetClass->GetName())));
	}
	else if (GridSize.X <= 0 || GridSize.Y <= 0)
	{
		CompileLog.Error(FText::Format(LOCTEXT("Error_TetrisGridPanel_InvalidGridSize", "Invalid grid size X: %d; Y: %d"), GridSize.X, GridSize.Y));
	}
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

