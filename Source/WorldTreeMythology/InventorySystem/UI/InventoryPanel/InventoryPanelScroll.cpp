// Copyright of Maple Game Studio


#include "InventoryPanelScroll.h"

void UInventoryPanelScroll::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!InventoryWidgetClass || !Panel) { return; }
	ResizePanel(MinEntryCount);
}

void UInventoryPanelScroll::SetupNavigation(UWidget* InWidget)
{
    if (Panel->Orientation == EOrientation::Orient_Vertical)
    {
        InWidget->SetNavigationRuleCustom(EUINavigation::Up, NavigationDelegate);
        InWidget->SetNavigationRuleCustom(EUINavigation::Down, NavigationDelegate);
    }
    else
    {
        InWidget->SetNavigationRuleCustom(EUINavigation::Left, NavigationDelegate);
        InWidget->SetNavigationRuleCustom(EUINavigation::Right, NavigationDelegate);
    }
}

void UInventoryPanelScroll::RefreshPanel(TArray<UInventoryEntry*> InQuery)
{
    // Resize Panel to the required number of children
    ResizePanel(GetRequiredEntryCount(InQuery.Num()));
}

void UInventoryPanelScroll::ResizePanel(int32 InEntryCount)
{
	if (InEntryCount < Panel->GetChildrenCount())
	{
		// Panel is oversized
		while (InEntryCount < Panel->GetChildrenCount() && Panel->GetChildrenCount() > MinEntryCount)
		{
			Entries.RemoveAt(Entries.Num() - 1);
			Panel->RemoveChildAt(Panel->GetChildrenCount() - 1);
		}
	}
	else if (InEntryCount > Panel->GetChildrenCount())
	{;
		//  MainPanel is undersized
		while (InEntryCount > Panel->GetChildrenCount())
		{
			UWidget* widget = AddNewWidget();
			Panel->AddChild(widget);
			SetupNavigation(widget);
		}
	}
}

void UInventoryPanelScroll::SetFocusedWidget(UInventoryWidget* InEntry)
{
	Super::SetFocusedWidget(InEntry);

	Panel->ScrollWidgetIntoView(InEntry,
		Panel->bAnimateWheelScrolling,
		Panel->NavigationDestination,
		Panel->NavigationScrollPadding);
}
