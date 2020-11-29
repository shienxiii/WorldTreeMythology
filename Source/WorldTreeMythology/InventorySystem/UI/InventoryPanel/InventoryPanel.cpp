// Copyright of Maple Game Studio


#include "InventoryPanel.h"
#include "Blueprint/WidgetTree.h"

UInventoryPanelBase::UInventoryPanelBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsFocusable = true;
    NavigationDelegate.BindUFunction(this, TEXT("NavigatePanel"));
}

UInventoryWidget* UInventoryPanelBase::AddNewWidget()
{
	if (!InventoryWidgetClass) { return nullptr; }

	int i = Entries.Add(CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass));
	//WidgetTree->ConstructWidget<UWidget>(InChildClass);
	Entries[i]->SetIndex(i);

	// Bind Callback Event
	Entries[i]->BindNativeOnEntryHovered(this, "NativeEntryHoverEvent");
	Entries[i]->BindNativeOnEntryClicked(this, "NativeEntryClickEvent");
	Entries[i]->BindOnAddedToFocus(this, "SetFocusedWidget");

	return Entries[i];
}

int32 UInventoryPanelBase::GetRequiredEntryCount(int32 InQueryCount)
{
	if (InventoryWidgetClass.Get()->IsChildOf<UInventoryButton>()) { return InQueryCount; }

	UInventoryPage* page = Cast<UInventoryPage>(InventoryWidgetClass.GetDefaultObject());

	int32 requiredEntryCount = InQueryCount / page->GetEntryCount();

	requiredEntryCount += ((InQueryCount % page->GetEntryCount()) > 0) ? 1 : 0;

	return requiredEntryCount;
}

int32 UInventoryPanelBase::GetEntryIndex(UInventoryWidget* InWidget)
{
	if (!InWidget) { return INDEX_NONE; }

	int32 index = Entries.IndexOfByPredicate([InWidget](const UWidget* widget)
		{
			return widget == InWidget;
		});

	return index;
}

void UInventoryPanelBase::QueryForList(TSubclassOf<AInventory> InInventoryClass)
{
	if (!InventoryComponent) { return; }
	RefreshPanel(InventoryComponent->GetInventoryListFor(InInventoryClass)->QueryForAll());
}

void UInventoryPanelBase::NativeEntryHoverEvent(UInventoryEntry* InEntry)
{
	EntryHoverEvent(InEntry);
	OnEntryHovered.Broadcast(InEntry);
}

void UInventoryPanelBase::NativeEntryClickEvent(UInventoryEntry* InEntry)
{
	EntryClickEvent(InEntry);
	OnEntryClicked.Broadcast(InEntry);
}

UWidget* UInventoryPanelBase::NavigatePanel(EUINavigation InNavigation)
{
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

	if (InventoryWidgetClass.Get()->IsChildOf<UInventoryButton>()) { return Entries[nextIndex]; }

	// Offset focused child
	UInventoryPage* prevPage = Cast<UInventoryPage>(Entries[prevIndex]);
	UInventoryPage* nextPage = Cast<UInventoryPage>(Entries[nextIndex]);

	int32 i = prevPage->GetEntryIndex(prevPage->GetFocusedEntry());

	return nextPage->GetEntryAt(i);
}

FReply UInventoryPanelBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (Entries.Num() == 0) { return FReply::Unhandled(); }

	if (FocusedEntry) { FocusedEntry->SetFocus(); }
	else { Entries[0]->SetFocus(); }

	return FReply::Handled();
}
