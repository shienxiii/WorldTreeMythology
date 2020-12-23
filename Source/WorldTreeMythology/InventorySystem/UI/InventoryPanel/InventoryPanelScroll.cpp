// Copyright of Maple Game Studio


#include "InventoryPanelScroll.h"

void UInventoryPanelScroll::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Warning, TEXT("NativeOnInitialized"));
}

void UInventoryPanelScroll::NativeDestruct()
{
	Super::NativeDestruct();
	Panel->ClearChildren();
	
	UE_LOG(LogTemp, Warning, TEXT("NativeDestruct"));

	GetWorld()->ForceGarbageCollection(true);
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
	Super::RefreshPanel(InQuery);

    // Resize Panel to the required number of children
    ResizePanel(GetRequiredEntryCount(InQuery.Num()));

	for (UInventoryWidget* entry : Entries)
	{
		entry->RefreshQuery(InQuery);
	}

	UE_LOG(LogTemp, Warning, TEXT("PanelCount %d"), Panel->GetChildrenCount());
	UE_LOG(LogTemp, Warning, TEXT("EntryCount %d"), Entries.Num());
}

void UInventoryPanelScroll::ResizePanel(int32 InEntryCount)
{
	if (!InventoryWidgetClass || !Panel) { return; }

	if (InEntryCount < Panel->GetChildrenCount() && InEntryCount > MinEntryCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ResizeDown"));
		// Panel is oversized
		while (InEntryCount < Panel->GetChildrenCount() && Panel->GetChildrenCount() > MinEntryCount)
		{
			Entries.RemoveAt(Entries.Num() - 1);
			Panel->RemoveChildAt(Panel->GetChildrenCount() - 1);
		}
	}
	else if (InEntryCount > Panel->GetChildrenCount() || MinEntryCount > Panel->GetChildrenCount())
	{
		//  MainPanel is undersized
		UE_LOG(LogTemp, Warning, TEXT("ResizeUp"));
		while (InEntryCount > Panel->GetChildrenCount() || MinEntryCount > Panel->GetChildrenCount())
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
