// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../../InventoryComponent.h"
#include "../InventoryButton.h"
#include "../InventoryPage/InventoryPage.h"
#include "InventoryPanel.generated.h"

/**
 * This class acts as the top-level of the Inventory UI and contains functions that allows communication between the UI system and the InventoryComponent.
 */
UCLASS(Abstract, NotBlueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Reference to the InventoryComponent that this instance will query
	UInventoryComponent* InventoryComponent = nullptr;

	// The InventoryWidget class that this InventoryPanel will fill Panel with
	UPROPERTY(EditAnywhere, Category = "Page Setup")TSubclassOf<UInventoryWidget> InventoryWidgetClass = NULL;

	// The minimum number of InventoryWidget to be present on Panel
	UPROPERTY(EditAnywhere, Category = "Page Setup") uint8 MinEntryCount = 1;

	TArray<UInventoryWidget*> Entries;
	UInventoryWidget* FocusedEntry = nullptr;

	// Delegate to be assigned to InventoryEntryButton upon creation
	FCustomWidgetNavigationDelegate NavigationDelegate;

public:
	UInventoryPanel(const FObjectInitializer& ObjectInitializer);

	UInventoryWidget* AddNewWidget();

	int32 GetRequiredEntryCount(int32 InQueryCount);
	int32 GetEntryIndex(UInventoryWidget* InWidget);


	UFUNCTION(BlueprintCallable) void SetInventoryComponent(UInventoryComponent* InInventoryComponent = nullptr) { InventoryComponent = InInventoryComponent; }

	UFUNCTION() virtual UWidget* NavigatePanel(EUINavigation InNavigation);
	UFUNCTION() void NativeEntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION() void NativeEntryClickEvent(UInventoryEntry* InEntry);
	UFUNCTION() void SetFocusedWidget(UInventoryWidget* InEntry) { FocusedEntry = Cast<UInventoryWidget>(InEntry); }

protected:
	virtual void SetupNavigation(UWidget* InWidget) { check(false); }
};
