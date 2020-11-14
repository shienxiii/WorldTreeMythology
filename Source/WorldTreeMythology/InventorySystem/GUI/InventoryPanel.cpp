// Copyright of Maple Game Studio


#include "InventoryPanel.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "InventoryPage.h"
#include "InventoryEntryButton.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Attempt to cast the MainPanel as a ScrollBox
	ScrollPanel = Cast<UScrollBox>(MainPanel);

	InitializePanel();

}

void UInventoryPanel::NativePreConstruct()
{
	InitializePanel();
}

void UInventoryPanel::InitializePanel()
{
	if (!MainPanel) { return; }

	MainPanel->ClearChildren();

	if (!InventoryWidget ||
		(!InventoryWidget->IsChildOf(UInventoryPage::StaticClass()) && !InventoryWidget->IsChildOf(UInventoryEntryButton::StaticClass()))
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
	int i = MainPanel->GetChildrenCount();

	UWidgetMk2* widget = Cast<UWidgetMk2>(AddChildToPanel(InventoryWidget));

	if (InventoryWidget->IsChildOf(UInventoryPage::StaticClass()))
	{
		UInventoryPage* page = Cast<UInventoryPage>(widget);

		page->BindHoverDelegate(this, "NativeEntryHoverEvent");
		page->BindClickDelegate(this, "NativeEntryClickEvent");
		page->SetPageNum(i);
	}
	else
	{
		UInventoryEntryButton* entry = Cast<UInventoryEntryButton>(widget);

		entry->BindHoverDelegate(this, "NativeEntryHoverEvent");
		entry->BindClickDelegate(this, "NativeEntryClickEvent");
		entry->SetQueryIndex(i);
	}

	BuildNavigation(widget);

	return widget;
}

void UInventoryPanel::QueryListFor(TSubclassOf<AInventory> InSubclass)
{
	if (!InventoryComponent) { return; }

	if (InSubclass == NULL) { InSubclass = DefaultQueriedSubclass; }

	if (UInventoryList* list = InventoryComponent->GetInventoryListFor(InSubclass))
	{
		TArray<UInventoryEntry*> entries = list->QueryForAll();
		RefreshPanel(entries);
	}
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

	TArray<UWidget*> widgets = MainPanel->GetAllChildren();

	for (UWidget* widget : widgets)
	{
		if (InventoryWidget->IsChildOf<UInventoryPage>())
		{
			Cast<UInventoryPage>(widget)->RefreshPage(InQueriedInventory);
		}
		else if (InventoryWidget.Get()->IsChildOf<UInventoryEntryButton>())
		{
			Cast<UInventoryEntryButton>(widget)->NativeRefresh(InQueriedInventory);
		}
	}
}

void UInventoryPanel::ResizePanel(TArray<UInventoryEntry*> InQueriedInventory)
{
	int32 queryCount = InQueriedInventory.Num();

	// Required children count will be equal to queryCount if MainPanel children are InventoryEntryDisplay
	int32 requiredChildrenCount = queryCount;

	// If MainPanel children are InventoryPage, recalculate the required number of children
	if (InventoryWidget.Get()->IsChildOf<UInventoryPage>())
	{
		UInventoryPage* page = Cast<UInventoryPage>(InventoryWidget.GetDefaultObject());

		requiredChildrenCount = queryCount / page->GetEntryCount();

		requiredChildrenCount += ((queryCount % page->GetEntryCount()) > 0) ? 1 : 0;
	}

	if(requiredChildrenCount < MainPanel->GetChildrenCount())
	{
		// MainPanel is oversized
		while (requiredChildrenCount < MainPanel->GetChildrenCount() && MainPanel->GetChildrenCount() > MinCount)
		{
			MainPanel->RemoveChildAt(MainPanel->GetChildrenCount() - 1);
		}
	}
	else if (requiredChildrenCount > MainPanel->GetChildrenCount())
	{
		//  MainPanel is undersized
		while (requiredChildrenCount > MainPanel->GetChildrenCount())
		{
			AddInventoryWidget();
		}
	}

}

void UInventoryPanel::SetFocusedChild(UWidget* InWidget)
{
	Super::SetFocusedChild(InWidget);

	if (ScrollPanel)
	{
		ScrollPanel->ScrollWidgetIntoView(FocusedChild, ScrollPanel->bAnimateWheelScrolling, ScrollPanel->NavigationDestination, ScrollPanel->NavigationScrollPadding);
	}
}

UWidget* UInventoryPanel::NavigateWidget(EUINavigation InNavigation)
{
	UWidget* nextWidget = Super::NavigateWidget(InNavigation);

	if (nextWidget->GetClass()->IsChildOf(UInventoryEntryButton::StaticClass()))
	{
		return nextWidget;
	}

	UInventoryPage* prevPage = Cast<UInventoryPage>(FocusedChild);
	UInventoryPage* nextPage = Cast<UInventoryPage>(nextWidget);
	check(prevPage && nextPage);

	int32 i = prevPage->GetChildIndex(prevPage->GetFocusedChild());
	check(i != INDEX_NONE);

	nextWidget = nextPage->GetChildAt(i);

	if (prevPage != nextPage) { prevPage->ForgetLastFocusedChild(); }

	return nextWidget;
}

void UInventoryPanel::PrevPage(bool bKeepOffset)
{
	if (!InventoryWidget.Get()->IsChildOf<UInventoryPage>()) { return; }

	UWidget* targetWidget = NavigateWidget(EUINavigation::Left);

	targetWidget->SetFocus();
}

void UInventoryPanel::NextPage(bool bKeepOffset)
{
	if (!InventoryWidget.Get()->IsChildOf<UInventoryPage>()) { return; }

	UWidget* targetWidget = NavigateWidget(EUINavigation::Right);

	targetWidget->SetFocus();
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
