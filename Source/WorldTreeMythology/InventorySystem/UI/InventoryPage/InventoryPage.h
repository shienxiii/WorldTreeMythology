// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../InventoryButton.h"
#include "InventoryPage.generated.h"

/**
 * This class holds a list of InventoryButton, and its children class will implement the Slate code to display the buttons on a panel widget.
 * 
 * InventoryButtons are added to this class based on the specified parameters. UI navigation is then initialized when the page is first drawn.
 */
UCLASS(Abstract, NotBlueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryPage : public UInventoryWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Page Setup") TSubclassOf<UInventoryButton> InventoryButtonClass = NULL;
	TArray<UInventoryButton*> Entries;
	UInventoryButton* FocusedEntry = nullptr;

	// Delegate to be assigned to InventoryEntryButton upon creation
	FCustomWidgetNavigationDelegate NavigationDelegate;
public:
	UInventoryPage(const FObjectInitializer& ObjectInitializer);
	~UInventoryPage();

	void NativeOnInitialized() override;
	void NativeDestruct() override;
	//void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetIndex(int32 InIndex = 0) override;

	void RefreshQuery(TArray<UInventoryEntry*> InEntry) override;

	int32 GetEntryIndex(UInventoryButton* InButton);
	UInventoryButton* GetEntryAt(int32 InIndex = 0);
	UInventoryButton* GetFocusedEntry() { return FocusedEntry; }
	void ClearFocusedWidget() { FocusedEntry = nullptr; }
	virtual int32 GetEntryCount() { return 0; }

protected:
	UInventoryButton* AddNewButton();
	virtual void SetupNavigation() { check(false); }

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION() virtual UWidget* NavigatePanel(EUINavigation InNavigation);
	UFUNCTION() void NativeEntryHoverEvent(UInventoryEntry* InEntry);
	UFUNCTION() void NativeEntryClickEvent(UInventoryEntry* InEntry);
	UFUNCTION() void SetFocusedEntry(UInventoryWidget* InEntry) { FocusedEntry = Cast<UInventoryButton>(InEntry); }
};
