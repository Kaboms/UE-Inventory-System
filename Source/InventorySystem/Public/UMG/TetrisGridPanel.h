// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Components/PanelWidget.h"
#include "TetrisGridPanel.generated.h"

class IWidgetCompilerLog;
class STetrisGridPanel;
class UTetrisGridSlot;

/**
 * A table-like panel that retains the width of every column throughout the table.
 *
 * * Many Children
 */
UCLASS()
class INVENTORYSYSTEM_API UTetrisGridPanel : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	UTetrisGridSlot* AddChildToGrid(UWidget* Content, int32 InRow = 0, int32 InColumn = 0);

	/** Gets the row index of the slot */
	float GetCellSize() const;

	/** Sets the row index of the slot, this determines what cell the slot is in the panel */
	UFUNCTION(BlueprintCallable)
	void SetCellSize(float InCellSize);

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetCellSize")
	float CellSize;

protected:
	TSharedPtr<STetrisGridPanel> MyTetrisGridPanel;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
