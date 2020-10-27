// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../CustomWidget/PanelWidgetMk2.h"
#include "../InventoryComponent.h"
#include "InventoryPanel.generated.h"

// This delegate is specifically declared to be used in BP to allow BP users to add delegate call to an InventoryPanel in when it is used in another UserWidget
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryEntryBPEvent, UInventoryEntry*, InEntry);

class UScrollBox;

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryPanel : public UPanelWidgetMk2
{
	GENERATED_BODY()
	
protected:
	// This is the child which will be spawned under MainPanel, should only be set to child of InventoryPage or InventoryEntryDisplay
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup")
		TSubclassOf<UWidgetMk2> InventoryWidget;

	// This is the minimum number of child to be in MainPanel
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup")
		uint8 MinCount = 1;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Panel Setup")
		TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Panel Setup")
		TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	UInventoryComponent* InventoryComponent;

	// This is the class this InventoryPanel will query for by default
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup") TSubclassOf<AInventory> DefaultQueriedSubclass = AInventory::StaticClass();

	// If MainPanel is a ScrollBox, this variable will point to it to avoid the need to check it everytime there is a need to perform any ScrollBox action
	UScrollBox* ScrollPanel = nullptr;

public:
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryHovered;
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryClicked;

public:
	void NativeOnInitialized() override;
	void NativePreConstruct() override;


	void InitializePanel();
	UWidget* AddInventoryWidget();

	// Set the InventoryComponent which this Panel will query from
	UFUNCTION(BlueprintCallable) void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }

	// Set the default Inventory class to query if the query are called without explicitly stating the InSubclass parameter
	UFUNCTION(BlueprintCallable) void SetDefaultQueriedClass(TSubclassOf<AInventory> InBaseClass) { DefaultQueriedSubclass = InBaseClass; }

	UFUNCTION(BlueprintCallable) void QueryListFor(TSubclassOf<AInventory> InSubclass = NULL);

	/**
	 * Query the InventoryComponent for InventoryEntry of objects that inherit from a subclass of Inventory.
	 * Automatically refreshes the panel
	 * 
	 * @param InSubclass The base subclass to query for
	 */
	UFUNCTION(BlueprintCallable) void QueryFor(TSubclassOf<AInventory> InSubclass = NULL);

	/**
	 * Query the InventoryComponent for InventoryEntry of objects that inherit from a subclass of Inventory with a custom condition.
	 * Automatically refreshes the panel
	 *
	 * @param InSubclass The base subclass to query for
	 */
	UFUNCTION(BlueprintCallable) void CustomQuery(TSubclassOf<AInventory> InSubclass = NULL, uint8 InQueryEnum = 0);

	/**
	 * Refresh this InventoryPanel to display a newly queried Inventory
	 * 
	 * @param InQueriedInventory The queried InventoryEntries to display
	 */
	void RefreshPanel(TArray<UInventoryEntry*>& InQueriedInventory);

	/**
	 * Add or remove children under MainPanel to fit the number of items queried
	 * 
	 * @param InQueriedInventory The queried InventoryEntries
	 */
	void ResizePanel(TArray<UInventoryEntry*> InQueriedInventory);


	void SetFocusedChild(UWidget* InWidget) override;


	virtual UWidget* NavigateWidget(EUINavigation InNavigation) override;

	/**
	 * If InventoryWidget is of type InventoryPage, go to the previous page.
	 * Optionally preserve the offset of the focused child
	 */
	UFUNCTION(BlueprintCallable) void PrevPage(bool bKeepOffset = true);

	/**
	 * If InventoryWidget is of type InventoryPage, go to the next page.
	 * Optionally preserve the offset of the focused child
	 */
	UFUNCTION(BlueprintCallable) void NextPage(bool bKeepOffset = true);

#pragma region Event
	UFUNCTION() void NativeEntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION(BlueprintImplementableEvent) void EntryHoverEvent(UInventoryEntry* InEntry);

	UFUNCTION() void NativeEntryClickEvent(UInventoryEntry* InEntry);
	UFUNCTION(BlueprintImplementableEvent) void EntryClickEvent(UInventoryEntry* InEntry);
#pragma endregion

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("Inventory UI"); }
#endif
};
