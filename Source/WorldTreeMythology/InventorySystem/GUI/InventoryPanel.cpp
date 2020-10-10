// Copyright of Maple Game Studio


#include "InventoryPanel.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "InventoryPage.h"
#include "InventoryEntryDisplay.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Attempt to cast the Panel as a ScrollBox
	ScrollPanel = Cast<UScrollBox>(Panel);

	BuildPanel();

}

void UInventoryPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (bIsEditor)
	{
		BuildPanel();
	}
}

void UInventoryPanel::BuildPanel()
{
	Panel->ClearChildren();

	if (!InventoryWidget ||
		(!InventoryWidget->IsChildOf(UInventoryPage::StaticClass()) && !InventoryWidget->IsChildOf(UInventoryEntryDisplay::StaticClass()))
		)
	{
		return;
	}
	

	for (int i = 0; i < MinCount; i++)
	{
		AddInventoryWidget();
	}
}

UWidget* UInventoryPanel::AddInventoryWidget()
{
	int i = Panel->GetChildrenCount();

	UWidgetMk2* widget = Cast<UWidgetMk2>(AddChildToPanel(InventoryWidget));
	widget->BindOnWidgetFocused(this, "SetLastFocusedChild");

	if (InventoryWidget->IsChildOf(UInventoryPage::StaticClass()))
	{
		UInventoryPage* page = Cast<UInventoryPage>(widget);
		page->BindHoverDelegate(this, "NativeEntryHoverEvent");
		page->BindClickDelegate(this, "NativeEntryClickEvent");
		page->SetPageNum(i);
	}
	else
	{
		UInventoryEntryDisplay* entry = Cast<UInventoryEntryDisplay>(widget);
		entry->BindHoverDelegate(this, "NativeEntryHoverEvent");
		entry->BindClickDelegate(this, "NativeEntryClickEvent");
		entry->SetQueryIndex(i);
	}

	BuildNavigation(widget);

	return widget;
}

TArray<UInventoryEntry*> UInventoryPanel::QueryForSubclass()
{
	return QueryForSubclass(QueryBaseClass);
}

TArray<UInventoryEntry*> UInventoryPanel::QueryForSubclass(TSubclassOf<AInventory> InSubclass)
{
	if (!InSubclass || !InventoryComponent)
	{
		return TArray<UInventoryEntry*>();
	}

	return InventoryComponent->QueryForSubclass(InSubclass);
}

TArray<UInventoryEntry*> UInventoryPanel::QueryForSubclass(TSubclassOf<AInventory> InSubclass, uint8 InQueryType)
{
	return TArray<UInventoryEntry*>();
}

UWidget* UInventoryPanel::NavigateWidget(EUINavigation InNavigation)
{
	UWidget* nextWidget = Super::NavigateWidget(InNavigation);

	if (nextWidget->GetClass()->IsChildOf(UInventoryEntryDisplay::StaticClass()))
	{
		return nextWidget;
	}

	UInventoryPage* prevPage = Cast<UInventoryPage>(LastFocusedChild);
	UInventoryPage* nextPage = Cast<UInventoryPage>(nextWidget);
	check(prevPage && nextPage);

	int32 i = prevPage->GetChildIndex(prevPage->GetLastFocusedChild());
	check(i != INDEX_NONE);

	nextWidget = nextPage->GetChildAt(i);
	prevPage->ForgetLastFocusedChild();

	return nextWidget;
}

void UInventoryPanel::SetLastFocusedChild(UWidget* InWidget)
{
	Super::SetLastFocusedChild(InWidget);

	if (ScrollPanel)
	{
		ScrollPanel->ScrollWidgetIntoView(LastFocusedChild, ScrollPanel->bAnimateWheelScrolling, ScrollPanel->NavigationDestination, ScrollPanel->NavigationScrollPadding);
	}
}

void UInventoryPanel::NativeEntryHoverEvent(UInventoryEntry* InEntry)
{
	EntryHoverEvent(InEntry);
}

void UInventoryPanel::NativeEntryClickEvent(UInventoryEntry* InEntry)
{
	EntryClickEvent(InEntry);
}
