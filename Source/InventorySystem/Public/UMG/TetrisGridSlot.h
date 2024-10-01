// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"
#include "Layout/Margin.h"
#include "Components/PanelSlot.h"
#include "Slate/STetrisGridPanel.h"

#include "TetrisGridSlot.generated.h"

/**
 * A slot for UTetrisGridPanel.
 */
UCLASS()
class INVENTORYSYSTEM_API UTetrisGridSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()
public:
	FMargin GetPadding() const;

	UFUNCTION(BlueprintCallable, Category = "Layout|Border Slot")
	void SetPadding(FMargin InPadding);

	/** Gets the row index of the slot */
	int32 GetRow() const;

	/** Sets the row index of the slot, this determines what cell the slot is in the panel */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetRow(int32 InRow);

	/** Gets the column index of the slot */
	int32 GetColumn() const;

	/** Sets the column index of the slot, this determines what cell the slot is in the panel */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetColumn(int32 InColumn);

	/** Gets the offset this cell drawn on top of others. */
	int32 GetLayer() const;

	/** Sets positive values offset this cell to be hit-tested and drawn on top of others. */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetLayer(int32 InLayer);

	/** Sets positive values offset this cell to be hit-tested and drawn on top of others. */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetSlotSize(FVector2D InSlotSize);

	FVector2D GetSlotSize() const;

	/** */
	EHorizontalAlignment GetHorizontalAlignment() const;

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	/**  */
	EVerticalAlignment GetVerticalAlignment() const;

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying FSlot for the Slate layout panel. */
	void BuildSlot(TSharedRef<STetrisGridPanel> TetrisGridPanel);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual bool NudgeByDesigner(const FVector2D& NudgeDirection, const TOptional<int32>& GridSnapSize) override;
	virtual void SynchronizeFromTemplate(const UPanelSlot* const TemplateSlot) override;
#endif //WITH_EDITOR

protected:
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetPadding", Category = "Layout|Tetris Grid Slot")
	FMargin Padding;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetHorizontalAlignment", Category = "Layout|Tetris Grid Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetVerticalAlignment", Category = "Layout|Tetris Grid Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	/** The row index of the cell this slot is in */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetRow", meta = (UIMin = "0"), Category = "Layout|Tetris Grid Slot")
	int32 Row;

	/** The column index of the cell this slot is in */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetColumn", meta = (UIMin = "0"), Category = "Layout|Tetris Grid Slot")
	int32 Column;

	/** Positive values offset this cell to be hit-tested and drawn on top of others. Default is 0; i.e. no offset. */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetLayer", Category = "Layout|Tetris Grid Slot")
	int32 Layer;

	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetSlotSize", Category = "Layout|Tetris Grid Slot", meta = (UIMin = "1", UIMax = "64", Delta = "1"))
	FVector2D SlotSize;

private:
	/** A raw pointer to the slot to allow us to adjust the size, padding...etc at runtime. */
	STetrisGridPanel::FSlot* Slot;
};
