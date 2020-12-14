// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../../InventoryComponent.h"
#include "../InventoryButton.h"
#include "../InventoryPage/InventoryPage.h"
#include "InventoryPanelBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryPanelBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Reference to the InventoryComponent that this instance will query
	UInventoryComponent* InventoryComponent = nullptr;

	// The InventoryWidget class that this InventoryPanel will fill Panel with
	UPROPERTY(EditAnywhere, Category = "Page Setup")TSubclassOf<UInventoryWidget> InventoryWidgetClass = NULL;

	// The minimum number of InventoryWidget to be present on Panel
	UPROPERTY(EditAnywhere, Category = "Page Setup") uint8 MinEntryCount = 1;

	bool bIsInitialized = false;

	TArray<UInventoryWidget*> Entries;
	UInventoryWidget* FocusedEntry = nullptr;

	FCustomWidgetNavigationDelegate NavigationDelegate;

public:
	FInventoryEntryBPEvent OnEntryHovered;
	FInventoryEntryBPEvent OnEntryClicked;


public:
	UInventoryPanelBase(const FObjectInitializer& ObjectInitializer);
	// NOTE: NativeOnInitialized is only called once, which is when a widget is created
	void NativeOnInitialized() override;
	// NOTE: Called everytime widget is undrawn
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable) void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }

	UInventoryWidget* AddNewWidget();
	int32 GetRequiredEntryCount(int32 InQueryCount);
	int32 GetEntryIndex(UInventoryWidget* InWidget);

#pragma region Query
	// Query for the content of the InventoryList that can hold InInventoryClass, including empty entries
	UFUNCTION(BlueprintCallable) void QueryForList(TSubclassOf<AInventory> InInventoryClass);

	virtual void RefreshPanel(TArray<UInventoryEntry*> InQuery) {}
#pragma endregion

	UFUNCTION() void NativeEntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION() void NativeEntryClickEvent(UInventoryEntry* InEntry);
	UFUNCTION(BlueprintImplementableEvent) void EntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION(BlueprintImplementableEvent) void EntryClickEvent(UInventoryEntry* InEntry);
	UFUNCTION() virtual void SetFocusedWidget(UInventoryWidget* InEntry) { FocusedEntry = Cast<UInventoryWidget>(InEntry); }
	UFUNCTION() virtual UWidget* NavigatePanel(EUINavigation InNavigation);
	UFUNCTION(BlueprintCallable) void ClearFocusedWidget();

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("Inventory UI"); }
#endif
};
