// Copyright of Maple Game Studio


#include "PanelMk2.h"
#include "Components/PanelWidget.h"
#include "Components/GridPanel.h"
#include "Blueprint/WidgetTree.h"

UPanelMk2::UPanelMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;

	NavigateNext.BindUFunction(this, TEXT("NavigateWidget"));
	NavigatePrev.BindUFunction(this, TEXT("NavigateWidget"));

	NavigateGrid.BindUFunction(this, TEXT("NavigateGridPanel"));
}

void UPanelMk2::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


void UPanelMk2::RebuildNavigation()
{
	for (UWidget* widget : MainPanel->GetAllChildren())
	{
		BuildNavigation(widget);
	}
}

void UPanelMk2::BuildNavigation(UWidget* InWidget)
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

UWidget* UPanelMk2::AddChildToPanel(TSubclassOf<UWidget> InChildClass)
{
	int i = MainPanel->GetChildrenCount();

	UWidget* newChild = WidgetTree->ConstructWidget<UWidget>(InChildClass);
	
	if (UGridPanel* grid = Cast<UGridPanel>(MainPanel))
	{
		//// Get x position
		//int x = i % GridX;

		//// Get y position
		//int y = i / GridX;

		//grid->AddChildToGrid(newChild, y, x);
	}
	else
	{
		MainPanel->AddChild(newChild);
	}


	if (UWidgetMk2* widget = Cast<UWidgetMk2>(newChild)) { widget->BindOnWidgetFocused(this, "SetFocusedChild"); }

	return newChild;
}

UWidget* UPanelMk2::GetChildAt(int32 ChildIndex)
{
	if (ChildIndex >= MainPanel->GetChildrenCount()) { return nullptr; }

	return MainPanel->GetChildAt(ChildIndex);
}

int32 UPanelMk2::GetChildIndex(UWidget* InWidget)
{
	if (!InWidget) { return INDEX_NONE; }

	int32 index = MainPanel->GetAllChildren().IndexOfByPredicate([InWidget](const UWidget* widget)
		{
			return widget == InWidget;
		});

	return index;
}

bool UPanelMk2::IsValidIndex(int32 InIndex)
{
	return MainPanel->GetChildAt(InIndex) && MainPanel->GetChildAt(InIndex)->bIsEnabled;
}

bool UPanelMk2::OnValidRow(int32 InIndex)
{
	/*int rowStart = (InIndex / GridX) * GridX;
	return IsValidIndex(rowStart);*/
	return false;
}

FReply UPanelMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Focus Received on %s"), *(GetClass()->GetFName().ToString()));

	if (FocusedChild)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(FocusedChild->GetClass()->GetFName().ToString()));
		//FocusedChild->SetFocus();
		MainPanel->GetChildAt(GetChildIndex(FocusedChild))->SetFocus();
	}
	else if(MainPanel->GetChildrenCount() > 0) { MainPanel->GetChildAt(0)->SetFocus(); }

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

UWidget* UPanelMk2::NavigateWidget(EUINavigation InNavigation)
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

UWidget* UPanelMk2::NavigateGridPanel(EUINavigation InNavigation)
{
	int32 index = GetChildIndex(FocusedChild);
	//if (index == INDEX_NONE) { return nullptr; }


	//// circular navigation for now
	//switch (InNavigation)
	//{
	//case EUINavigation::Left:
	//	do { index -= (index % GridX) == 0 ? -(GridX - 1) : 1; } while (!IsValidIndex(index));
	//	break;
	//case EUINavigation::Right:
	//	do { index += (index % GridX) == (GridX - 1) ? -(GridX - 1) : 1; } while (!IsValidIndex(index));
	//	break;
	//case EUINavigation::Up:
	//	do
	//	{
	//		index -= GridX;
	//		index = index > -1 ? index : (GridX * GridY) + index;
	//	} while (!OnValidRow(index));

	//	while (!IsValidIndex(index)) { index -= (index % GridX) == 0 ? -(GridX - 1) : 1; }

	//	break;
	//case EUINavigation::Down:
	//	do
	//	{
	//		index += GridX;
	//		index %= (GridX * GridY);
	//	} while (!OnValidRow(index));

	//	while (!IsValidIndex(index)) { index -= (index % GridX) == 0 ? -(GridX - 1) : 1; }

	//	break;
	//}

	return MainPanel->GetChildAt(index);
}


