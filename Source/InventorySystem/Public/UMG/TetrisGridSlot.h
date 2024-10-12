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
	GENERATED_BODY()

public:
	UTetrisGridSlot(const FObjectInitializer& ObjectInitializer);

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

	/** Sets positive values offset this cell to be hit-tested and drawn on top of others. */
	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetSlotSize(FVector2D InSlotSize);

	FVector2D GetSlotSize() const;

	UFUNCTION(BlueprintCallable, Category = "Layout|Tetris Grid Slot")
	void SetSlotForm(TSet<FVector2D> SlotForm);

	TSet<FVector2D> GetSlotForm() const;

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

	UFUNCTION(BlueprintPure, Category = "Slot")
	static UTetrisGridSlot* SlotAsTetrisGridSlot(UWidget* Widget);

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

	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetSlotSize", Category = "Layout|Tetris Grid Slot", meta = (UIMin = "1", UIMax = "64", Delta = "1"))
	FVector2D SlotSize;

	// Use this for exact slot form. By default slot is Rectangle setted by SlotSize property
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetSlotForm", Category = "Layout|Tetris Grid Slot")
	TSet<FVector2D> SlotForm;

private:
	/** A raw pointer to the slot to allow us to adjust the size, padding...etc at runtime. */
	STetrisGridPanel::FSlot* Slot;
};
