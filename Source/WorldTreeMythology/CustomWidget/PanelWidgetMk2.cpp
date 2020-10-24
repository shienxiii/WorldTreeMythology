// Copyright of Maple Game Studio


#include "PanelWidgetMk2.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"

UPanelWidgetMk2::UPanelWidgetMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;

	NavigateNext.BindUFunction(this, TEXT("NavigateWidget"));
	NavigatePrev.BindUFunction(this, TEXT("NavigateWidget"));
}

void UPanelWidgetMk2::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsEditor = false; // Stop editor preview code from running
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
	if (NavigationDirection == EPanelLayout::HORIZONTAL)
	{
		InWidget->SetNavigationRuleCustom(EUINavigation::Left, NavigatePrev);
		InWidget->SetNavigationRuleCustom(EUINavigation::Right, NavigateNext);
	}
	else if (NavigationDirection == EPanelLayout::VERTICAL)
	{
		InWidget->SetNavigationRuleCustom(EUINavigation::Up, NavigatePrev);
		InWidget->SetNavigationRuleCustom(EUINavigation::Down, NavigateNext);
	}
}

UWidget* UPanelWidgetMk2::AddChildToPanel(TSubclassOf<UWidget> InChildClass)
{
	UWidget* newChild = WidgetTree->ConstructWidget<UWidget>(InChildClass);
	MainPanel->AddChild(newChild);
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

UWidget* UPanelWidgetMk2::NavigateWidget(EUINavigation InNavigation)
{
	int32 index = GetChildIndex(LastFocusedChild);
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