// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../../CustomWidget/PanelWidgetMk2.h"
#include "InventoryEntryDisplay.h"
#include "InventoryPage.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryPage : public UPanelWidgetMk2
{
	GENERATED_BODY()
	
protected:
	// This is the InventoryEntryDisplay class that this page will be built with
	UPROPERTY(EditAnywhere, Category = "Panel Setup") TSubclassOf<UInventoryEntryDisplay> EntryClass = NULL;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup", meta = (EditCondition = "PanelLayout != EPanelLayout::GRID")) uint8 EntryCount = 5;

	UPROPERTY(BlueprintReadOnly) int32 PageNum = -1;

	FInventoryEntryEvent OnEntryHovered;
	FInventoryEntryEvent OnEntryClicked;
public:
	void NativeOnInitialized() override;
	void NativePreConstruct() override;
	void InitializePage();

	uint8 GetEntryCount() { return PanelLayout == EPanelLayout::GRID ? GridX * GridY : EntryCount; }
	void SetPageNum(int32 InPageNum);

	void RefreshPage(TArray<UInventoryEntry*> InQuery);

	bool BindHoverDelegate(UObject* InObject, FName InFuncName);
	bool BindClickDelegate(UObject* InObject, FName InFuncName);


	UFUNCTION() void EntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION() void EntryClickEvent(UInventoryEntry* InEntry);
#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("Inventory UI"); }
#endif
};
