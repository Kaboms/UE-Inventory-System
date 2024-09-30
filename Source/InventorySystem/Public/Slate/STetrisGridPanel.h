// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SlotBase.h"
#include "Widgets/SWidget.h"
#include "Layout/Children.h"
#include "Widgets/SPanel.h"

class FArrangedChildren;
class FPaintArgs;
class FSlateWindowElementList;

// Tetris-like grid panel. Most logic "borrowed" from SGridPanel
class INVENTORYSYSTEM_API STetrisGridPanel : public SPanel
{
	SLATE_DECLARE_WIDGET(STetrisGridPanel, SPanel)

public:
	// Used by the mandatory named parameter in FSlot
	class Layer
	{
	public:
		explicit Layer(int32 InLayer)
			: TheLayer(InLayer)
		{

		}

		int32 TheLayer;
	};

	class INVENTORYSYSTEM_API FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
		friend STetrisGridPanel;

	public:
		/** Default values for a slot. */
		FSlot(int32 Column, int32 Row, FVector2D SlotSize, int32 InLayer)
			: TBasicLayoutWidgetSlot<FSlot>(HAlign_Fill, VAlign_Fill)
			, ColumnParam(Column)
			, RowParam(Row)
			, SlotSizeParam(SlotSize)
			, LayerParam(InLayer)
		{
		}

		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
		/** Which column in the grid this cell belongs to */
		SLATE_ARGUMENT(TOptional<int32>, Column)
		/** Which row in the grid this cell belongs to */
		SLATE_ARGUMENT(TOptional<int32>, Row)
		/** Positive values offset this cell to be hit-tested and drawn on top of others. Default is 0; i.e. no offset. */
		SLATE_ARGUMENT(TOptional<int32>, Layer)
		/** Cell size */
		SLATE_ARGUMENT(TOptional<FVector2D>, SlotSize)

		SLATE_SLOT_END_ARGS()

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs);

	public:
		/** Which column in the grid this cell belongs to */
		int32 GetColumn() const
		{
			return ColumnParam;
		}

		void SetColumn(int32 Column)
		{
			Column = FMath::Max(0, Column);
			if (Column != ColumnParam)
			{
				ColumnParam = Column;
				NotifySlotChanged();
			}
		}

		/** Which row in the grid this cell belongs to */
		int32 GetRow() const
		{
			return RowParam;
		}

		void SetRow(int32 Row)
		{
			Row = FMath::Max(0, Row);
			if (Row != RowParam)
			{
				RowParam = Row;
				NotifySlotChanged();
			}
		}

		/** Positive values offset this cell to be hit-tested and drawn on top of others. Default is 0; i.e. no offset. */
		int32 GetLayer() const
		{
			return LayerParam;
		}

		void SetLayer(int32 Layer)
		{
			if (Layer != LayerParam)
			{
				LayerParam = Layer;
				const bool bSlotLayerChanged = true;
				NotifySlotChanged(bSlotLayerChanged);
			}
		}

		FVector2D GetSlotSize() const
		{
			return SlotSizeParam;
		}

		void SetSlotSize(FVector2D SlotSize)
		{
			if (SlotSize != SlotSizeParam)
			{
				SlotSizeParam = SlotSize;
				NotifySlotChanged();
			}
		}

	private:
		/** The panel that contains this slot */
		TWeakPtr<STetrisGridPanel> Panel;

		int32 ColumnParam;
		int32 RowParam;
		int32 LayerParam;

		FVector2D SlotSizeParam;

		/** Notify that the slot was changed */
		FORCEINLINE void NotifySlotChanged(bool bSlotLayerChanged = false)
		{
			if (Panel.IsValid())
			{
				Panel.Pin()->NotifySlotChanged(this, bSlotLayerChanged);
			}
		}
	};

	/**
	 * Used by declarative syntax to create a Slot in the specified Column, Row and Layer.
	 */
	static FSlot::FSlotArguments Slot(int32 Column, int32 Row, FVector2D SlotSize, Layer InLayer = Layer(0));

	using FScopedWidgetSlotArguments = TPanelChildren<FSlot>::FScopedWidgetSlotArguments;
	/**
	 * Dynamically add a new slot to the UI at specified Column and Row. Optionally, specify a layer at which this slot should be added.
	 *
	 * @return A reference to the newly-added slot
	 */
	FScopedWidgetSlotArguments AddSlot(int32 Column, int32 Row, FVector2D SlotSize, Layer InLayer = Layer(0));

	/**
	* Removes a slot from this panel which contains the specified SWidget
	*
	* @param SlotWidget The widget to match when searching through the slots
	* @returns The true if the slot was removed and false if no slot was found matching the widget
	*/
	bool RemoveSlot(const TSharedRef<SWidget>& SlotWidget);

	SLATE_BEGIN_ARGS(STetrisGridPanel)
		: _GridSize(FVector2D(6, 6))
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SLOT_ARGUMENT(FSlot, Slots)

		SLATE_ARGUMENT(FVector2D, GridSize)

	SLATE_END_ARGS()

	STetrisGridPanel();

	/** Removes all slots from the panel */
	void ClearChildren();

	void Construct(const FArguments& InArgs);

	/**
	 * GetDesiredSize of a subregion in the graph.
	 *
	 * @param StartCell   The cell (inclusive) in the upper left of the region.
	 * @param Size        Number of cells in the X and Y directions to get the size for.
	 *
	 * @return FVector2D  The desired size of the region of cells specified.
	 */
	FVector2D GetDesiredRegionSize(const FIntPoint& StartCell, int32 Width, int32 Height) const;

	void SetGridSize(FVector2D GridSize);

public:

	// SWidget interface

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual void CacheDesiredSize(float) override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual FChildren* GetChildren() override;

private:

	/**
	 * Given an array of values, re-populate the array such that every contains the partial sums up to that element.
	 * i.e. Array[N] = Array.Sum(0 .. N-1)
	 *
	 * The resulting array is 1-element longer.
	 */
	static void ComputePartialSums(TArray<float>& TurnMeIntoPartialSums);

	/** Given a SizeContribution, distribute it to the elements in DistributeOverMe at indexes from [StartIndex .. UpperBound) */
	static void DistributeSizeContributions(float SizeContribution, TArray<float>& DistributeOverMe, int32 StartIndex, int32 UpperBound);

	/**
	 * Find the index where the given slot should be inserted into the list of Slots based on its LayerParam, such that Slots are sorter by layer.
	 *
	 * @param The newly-allocated slot to insert.
	 * @return The index where the slot should be inserted.
	 */
	int32 FindInsertSlotLocation(const FSlot* InSlot);

	/** Compute the sizes of columns and rows needed to fit all the slots in this grid. */
	void ComputeDesiredCellSizes(TArray<float>& OutColumns, TArray<float>& OutRows) const;

	/** Draw the debug grid of rows and colummns; useful for inspecting the GridPanel's logic. See OnPaint() for parameter meaning */
	int32 LayoutDebugPaint(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	/**
	 * Callback used to resize our internal arrays if a slot (or slot span) is changed.
	 *
	 * @param InSlot The slot that has just changed.
	 * @param bSlotLayerChanged Whether the slot layer changed.
	 */
	void NotifySlotChanged(const FSlot* InSlot, bool bSlotLayerChanged = false);

private:

	/** The slots that are placed into various grid locations */
	TPanelChildren<FSlot> Slots;

	/**
	 * Offsets of each column from the beginning of the grid.
	 * Includes a faux value at the end of the array for finding the size of the last cell.
	 */
	TArray<float> Columns;

	/**
	 * Offsets of each row from the beginning of the grid.
	 * Includes a faux value at the end of the array for finding the size of the last cell.
	 */
	TArray<float> Rows;

	/** Total desires size along each axis. */
	FVector2D TotalDesiredSizes;

	FVector2D GridSize;
};
