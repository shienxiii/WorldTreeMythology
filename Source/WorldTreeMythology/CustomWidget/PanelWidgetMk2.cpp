// Copyright of Maple Game Studio


#include "PanelWidgetMk2.h"
#include "Components/PanelWidget.h"
#include "Components/GridPanel.h"
#include "Blueprint/WidgetTree.h"

UPanelWidgetMk2::UPanelWidgetMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;

	NavigateNext.BindUFunction(this, TEXT("NavigateWidget"));
	NavigatePrev.BindUFunction(this, TEXT("NavigateWidget"));

	NavigateGrid.BindUFunction(this, TEXT("NavigateGridPanel"));
}

void UPanelWidgetMk2::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UPanelWidgetMk2::RebuildNavigation()
{
	for (UWidget* widget : MainPanel->GetAllChildren())
	{
		BuildNavigation(widget);
	}
}

void UPanelWidgetMk2::BuildNavigation(UWidget* InWidget)
{
	if (PanelLayout == EPanelLayout::HORIZONTAL)
	{
		InWidget->SetNavigationRuleCustom(EUINavigation::Left, NavigatePrev);
		InWidget->SetNavigationRuleCustom(EUINavigation::Right, NavigateNext);
	}
	else if (PanelLayout == EPanelLayout::VERTICAL)
	{
		InWidget->SetNavigationRuleCustom(EUINavigation::Up, NavigatePrev);
		InWidget->SetNavigationRuleCustom(EUINavigation::Down, NavigateNext);
	}
	else if (PanelLayout == EPanelLayout::GRID)
	{
		InWidget->SetNavigationRuleCustom(EUINavigation::Up, NavigateGrid);
		InWidget->SetNavigationRuleCustom(EUINavigation::Down, NavigateGrid);
		InWidget->SetNavigationRuleCustom(EUINavigation::Left, NavigateGrid);
		InWidget->SetNavigationRuleCustom(EUINavigation::Right, NavigateGrid);
	}
}

UWidget* UPanelWidgetMk2::AddChildToPanel(TSubclassOf<UWidget> InChildClass)
{
	int i = MainPanel->GetChildrenCount();

	UWidget* newChild = WidgetTree->ConstructWidget<UWidget>(InChildClass);

	if (UGridPanel* grid = Cast<UGridPanel>(MainPanel))
	{
		// Get x position
		int x = i % GridX;

		// Get y position
		int y = i / GridX;

		grid->AddChildToGrid(newChild, y, x);
		//PlaceOnGrid(newChild,)
	}
	else
	{
		MainPanel->AddChild(newChild);
	}


	if (UWidgetMk2* widget = Cast<UWidgetMk2>(newChild)) { widget->BindOnWidgetFocused(this, "SetFocusedChild"); }

	return newChild;
}

UWidget* UPanelWidgetMk2::GetChildAt(int32 ChildIndex)
{
	if (ChildIndex >= MainPanel->GetChildrenCount()) { return nullptr; }

	return MainPanel->GetChildAt(ChildIndex);
}

int32 UPanelWidgetMk2::GetChildIndex(UWidget* InWidget)
{
	if (!InWidget) { return INDEX_NONE; }

	int32 index = MainPanel->GetAllChildren().IndexOfByPredicate([InWidget](const UWidget* widget)
		{
			return widget == InWidget;
		});

	return index;
}

FReply UPanelWidgetMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (FocusedChild) { FocusedChild->SetFocus(); }
	else if (MainPanel->GetChildrenCount() > 0) { MainPanel->GetChildAt(0)->SetFocus(); }

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

UWidget* UPanelWidgetMk2::NavigateWidget(EUINavigation InNavigation)
{
	int32 index = GetChildIndex(FocusedChild);
	if (index == INDEX_NONE) { return nullptr; }

	// circular navigation for now
	if (InNavigation == EUINavigation::Down || InNavigation == EUINavigation::Right)
	{
		index++;
		index %= MainPanel->GetChildrenCount();
	}
	else
	{
		index--;
		index = index > -1 ? index : MainPanel->GetChildrenCount() - 1;
	}

	return MainPanel->GetChildAt(index);
}

UWidget* UPanelWidgetMk2::NavigateGridPanel(EUINavigation InNavigation)
{
	int32 index = GetChildIndex(FocusedChild);
	if (index == INDEX_NONE) { return nullptr; }

	// circular navigation for now
	switch (InNavigation)
	{
	case EUINavigation::Left:
		index -= (index % GridX) == 0 ? -(GridX - 1) : 1;
		break;
	case EUINavigation::Right:
		index += (index % GridX) == (GridX - 1) ? -(GridX - 1) : 1;
		break;
	case EUINavigation::Up:
		index -= GridX;
		index = index > -1 ? index : MainPanel->GetChildrenCount() + index;
		break;
	case EUINavigation::Down:
		index += GridX;
		index %= MainPanel->GetChildrenCount();
		break;

	}

	return MainPanel->GetChildAt(index);
}
