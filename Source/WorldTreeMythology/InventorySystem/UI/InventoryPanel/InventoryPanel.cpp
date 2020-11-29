// Copyright of Maple Game Studio


#include "InventoryPanel.h"

UInventoryPanel::UInventoryPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
	NavigationDelegate.BindUFunction(this, TEXT("NavigatePanel"));
}

UInventoryWidget* UInventoryPanel::AddNewWidget()
{
	if (!InventoryWidgetClass) { return nullptr; }

	int i = Entries.Add(CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass));

	// Bind Callback Event
	Entries[i]->BindNativeOnEntryHovered(this, "NativeEntryHoverEvent");
	Entries[i]->BindNativeOnEntryClicked(this, "NativeEntryClickEvent");
	Entries[i]->BindOnAddedToFocus(this, "SetFocusedWidget");

	//SetupNavigation(Entries[i]);

	return Entries[i];
}

int32 UInventoryPanel::GetRequiredEntryCount(int32 InQueryCount)
{
	if (InventoryWidgetClass.Get()->IsChildOf<UInventoryButton>()) { return InQueryCount; }

	UInventoryPage* page = Cast<UInventoryPage>(InventoryWidgetClass.GetDefaultObject());

	int32 requiredChildrenCount = InQueryCount / page->GetEntryCount();

	requiredChildrenCount += ((InQueryCount % page->GetEntryCount()) > 0) ? 1 : 0;

	return requiredChildrenCount;
}

int32 UInventoryPanel::GetEntryIndex(UInventoryWidget* InWidget)
{
	if (!InWidget) { return INDEX_NONE; }

	int32 index = Entries.IndexOfByPredicate([InWidget](const UWidget* widget)
		{
			return widget == InWidget;
		});

	return index;
}

UWidget* UInventoryPanel::NavigatePanel(EUINavigation InNavigation)
{
	UE_LOG(LogTemp, Warning, TEXT("NavigatePanel"));

	int32 prevIndex = GetEntryIndex(FocusedEntry);
	int32 nextIndex = prevIndex;

	if (prevIndex == INDEX_NONE) { return nullptr; }

	// circular navigation for now
	if (InNavigation == EUINavigation::Down || InNavigation == EUINavigation::Right)
	{
		nextIndex++;
		nextIndex %= Entries.Num();
	}
	else
	{
		nextIndex--;
		nextIndex = nextIndex > -1 ? nextIndex : Entries.Num() - 1;
	}

	if (InventoryWidgetClass.Get()->IsChildOf<UInventoryButton>()) { UE_LOG(LogTemp, Warning, TEXT("Processing Button")); return Entries[nextIndex]; }

	UE_LOG(LogTemp, Warning, TEXT("Processing Pages"));
	// Offset focused child
	UInventoryPage* prevPage = Cast<UInventoryPage>(Entries[prevIndex]);
	UInventoryPage* nextPage = Cast<UInventoryPage>(Entries[nextIndex]);

	int32 i = prevPage->GetEntryIndex(prevPage->GetFocusedEntry());

	return nextPage->GetEntryAt(i);
}

void UInventoryPanel::NativeEntryHoverEvent(UInventoryEntry* InEntry)
{
}

void UInventoryPanel::NativeEntryClickEvent(UInventoryEntry* InEntry)
{
}
