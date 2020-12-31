// Copyright of Maple Game Studio


#include "InventoryPanelBase.h"
#include "Blueprint/WidgetTree.h"

UInventoryPanelBase::UInventoryPanelBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsFocusable = true;
    NavigationDelegate.BindUFunction(this, TEXT("NavigatePanel"));
}

void UInventoryPanelBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsInitialized = true;
}

void UInventoryPanelBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	// The purpose of this block of code is only to preview on the UMG editor
	if (!bIsInitialized)
	{
		ResizePanel(MinEntryCount);

		return;
	}

	if (!InventoryComponent || !bAutoQuery || !InventoryClass) { return; }

	QueryForList(InventoryClass);
}

void UInventoryPanelBase::NativeDestruct()
{
	Super::NativeDestruct();
	ClearFocusedWidget();
	Entries.Empty();
}

UInventoryWidget* UInventoryPanelBase::AddNewWidget()
{
	if (!InventoryWidgetClass) { return nullptr; }

	int i = Entries.Add(CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass));

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

void UInventoryPanelBase::QueryForList(TSubclassOf<AInventoryObject> InInventoryClass)
{
	if (!InventoryComponent) { return; }

	RefreshPanel(InventoryComponent->QueryByBaseClass(InInventoryClass));
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
	if (!FocusedEntry) { return nullptr; }

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

	if (i == INDEX_NONE) { return nullptr; }

	// If next page's entry at i is valid and enabled, return the entry
	if(!nextPage->GetEntryAt(i)->IsNullEntry()) 
	{ 
		return nextPage->GetEntryAt(i);
	}

	do
	{
		i--;
	} while (i > 0 && nextPage->GetEntryAt(i)->IsNullEntry());


	return !(nextPage->GetEntryAt(i)->IsNullEntry()) ? nextPage->GetEntryAt(i) : prevPage->GetFocusedEntry();
}

void UInventoryPanelBase::ClearFocusedWidget()
{
	FocusedEntry = nullptr;

	if (InventoryWidgetClass.Get()->IsChildOf<UInventoryButton>()) { return; }

	for (UInventoryWidget* entry : Entries)
	{
		Cast<UInventoryPage>(entry)->ClearFocusedWidget();
	}
}

FReply UInventoryPanelBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (Entries.Num() == 0) { return FReply::Unhandled(); }

	if (FocusedEntry)
	{
		FString name = FocusedEntry->GetFName().ToString();

		FocusedEntry->SetFocus();
	}
	else { Entries[0]->SetFocus(); }

	return FReply::Handled();
}
