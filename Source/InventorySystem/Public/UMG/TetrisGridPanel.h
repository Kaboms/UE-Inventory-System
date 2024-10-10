// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Components/PanelWidget.h"
#include "Slate/STetrisGridPanel.h"
#include "Blueprint/UserWidgetPool.h"
#include "TetrisGridPanel.generated.h"

class IWidgetCompilerLog;
class UTetrisGridSlot;

/**
 * Tetris-like panel with a fixed grid slot size
 *
 * * Many Children
 */
UCLASS(meta = (SlotInterface = TetrisSlot))
class INVENTORYSYSTEM_API UTetrisGridPanel : public UPanelWidget
{
	GENERATED_UCLASS_BODY()

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Widget")
	UTetrisGridSlot* AddChildToGrid(UWidget* Content, int32 InRow = 0, int32 InColumn = 0);

	/** Gets the row index of the slot */
	FVector2D GetGridSize() const;

	/** Sets the row index of the slot, this determines what cell the slot is in the panel */
	UFUNCTION(BlueprintCallable)
	void SetGridSize(FVector2D InGridSize);

	UFUNCTION(BlueprintGetter)
	TSubclassOf<UUserWidget> GetSlotWidgetClass() const { return SlotWidgetClass; }

	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif

protected:
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

private:
	TSharedRef<SWidget> HandleGenerateTetrisSlot();

protected:
	UPROPERTY(EditAnywhere, Getter, Setter, BlueprintSetter = "SetGridSize", meta = (UIMin = "1", UIMax = "64", Delta = "1"))
	FVector2D GridSize;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetSlotWidgetClass, Category = Slots, meta = (DesignerRebuild, AllowPrivateAccess = true, MustImplement = "/Script/InventorySystem.TetrisSlot"))
	TSubclassOf<UUserWidget> SlotWidgetClass;

	TSharedPtr<STetrisGridPanel> MyTetrisGridPanel;

	UPROPERTY(Transient)
	FUserWidgetPool EntryWidgetPool;

private:
	friend class FTetrisGridPanelDetails;
};
