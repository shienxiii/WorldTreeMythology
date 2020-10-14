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
	// This is the child which will be spawned under Panel, should only be set to child of InventoryPage or InventoryEntryDisplay
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup")
		TSubclassOf<UWidgetMk2> InventoryWidget;

	// This is the minimum number of child to be in Panel
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

	// If Panel is a ScrollBox, this variable will point to it to avoid the need to check it everytime there is a need to perform any ScrollBox action
	UScrollBox* ScrollPanel = nullptr;

public:
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryHovered;
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryClicked;

public:
	void NativeOnInitialized() override;
	void SynchronizeProperties() override;


	void InitializePanel();
	UWidget* AddInventoryWidget();

	UFUNCTION(BlueprintCallable) void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }
	UFUNCTION(BlueprintCallable) void SetDefaultQueriedClass(TSubclassOf<AInventory> InBaseClass) { DefaultQueriedSubclass = InBaseClass; }


	UFUNCTION(BlueprintCallable) void QueryFor(TSubclassOf<AInventory> InSubclass = NULL);
	UFUNCTION(BlueprintCallable) void FilteredQuery(TSubclassOf<AInventory> InSubclass = NULL, uint8 InFilterEnum = 0);
	void RefreshPanel(TArray<UInventoryEntry*> InQueriedInventory);


	virtual UWidget* NavigateWidget(EUINavigation InNavigation) override;
	void SetLastFocusedChild(UWidget* InWidget) override;

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
