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

	InitializePanel();

}

void UInventoryPanel::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (bIsEditor)
	{
		InitializePanel();
	}
}

void UInventoryPanel::InitializePanel()
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

void UInventoryPanel::QueryFor(TSubclassOf<AInventory> InSubclass)
{
	if (!InventoryComponent) { return; }

	if (InSubclass == NULL) { InSubclass = DefaultQueriedSubclass; }

	TArray<UInventoryEntry*> entries = InventoryComponent->QueryForSubclass(InSubclass);

	RefreshPanel(entries);
}

void UInventoryPanel::CustomQuery(TSubclassOf<AInventory> InSubclass, uint8 InCustomEnum)
{
	if (!InventoryComponent) { return; }

	if (InSubclass == NULL) { InSubclass = DefaultQueriedSubclass; }

	TArray<UInventoryEntry*> entries = InventoryComponent->CustomQuery(InSubclass, InCustomEnum);

	RefreshPanel(entries);
}

void UInventoryPanel::RefreshPanel(TArray<UInventoryEntry*>& InQueriedInventory)
{
	ResizePanel(InQueriedInventory);

	TArray<UWidget*> widgets = Panel->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		if (InventoryWidget->IsChildOf<UInventoryPage>())
		{
			Cast<UInventoryPage>(widget)->RefreshPage(InQueriedInventory);
		}
		else if (InventoryWidget.Get()->IsChildOf<UInventoryEntryDisplay>())
		{
			Cast<UInventoryEntryDisplay>(widget)->NativeRefresh(InQueriedInventory);
		}
	}
}

void UInventoryPanel::ResizePanel(TArray<UInventoryEntry*> InQueriedInventory)
{
	int32 queryCount = InQueriedInventory.Num();

	// Required children count will be equal to queryCount if Panel children are InventoryEntryDisplay
	int32 requiredChildrenCount = queryCount;

	// If Panel children are InventoryPage, recalculate the required number of children
	if (InventoryWidget.Get()->IsChildOf<UInventoryPage>())
	{
		UInventoryPage* page = Cast<UInventoryPage>(InventoryWidget.GetDefaultObject());

		requiredChildrenCount = queryCount / page->GetEntryCount();

		requiredChildrenCount += ((queryCount % page->GetEntryCount()) > 0) ? 1 : 0;
	}

	if(requiredChildrenCount < Panel->GetChildrenCount())
	{
		// Panel is oversized
		while (requiredChildrenCount < Panel->GetChildrenCount() && Panel->GetChildrenCount() > MinCount)
		{
			Panel->RemoveChildAt(Panel->GetChildrenCount() - 1);
		}
	}
	else if (requiredChildrenCount > Panel->GetChildrenCount())
	{
		//  Panel is undersized
		while (requiredChildrenCount > Panel->GetChildrenCount())
		{
			AddInventoryWidget();
		}
	}

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

	if (prevPage != nextPage) { prevPage->ForgetLastFocusedChild(); }

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
	OnEntryHovered.Broadcast(InEntry);
}

void UInventoryPanel::NativeEntryClickEvent(UInventoryEntry* InEntry)
{
	EntryClickEvent(InEntry);
	OnEntryClicked.Broadcast(InEntry);
}
