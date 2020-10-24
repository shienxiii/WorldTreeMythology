// Copyright of Maple Game Studio


#include "InventoryPage.h"


void UInventoryPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InitializePage();
}

void UInventoryPage::NativePreConstruct()
{
	InitializePage();
}

void UInventoryPage::InitializePage()
{
	if (!EntryClass || !MainPanel) { return; }

	MainPanel->ClearChildren();

	for (int i = 0; i < EntryCount; i++)
	{
		UInventoryEntryDisplay* entry = Cast<UInventoryEntryDisplay>(AddChildToPanel(EntryClass));
		entry->BindOnWidgetFocused(this, "SetLastFocusedChild");
		entry->BindHoverDelegate(this, "EntryHoverEvent");
		entry->BindClickDelegate(this, "EntryClickEvent");
		BuildNavigation(entry);
	}
}

void UInventoryPage::SetPageNum(int32 InPageNum)
{
	PageNum = InPageNum;
	TArray<UWidget*> entries = MainPanel->GetAllChildren();

	for (int i = 0; i < entries.Num(); i++)
	{
		Cast<UInventoryEntryDisplay>(entries[i])->SetQueryIndex((PageNum * EntryCount) + i);
	}
}

void UInventoryPage::RefreshPage(TArray<UInventoryEntry*> InQuery)
{
	TArray<UWidget*> entries = MainPanel->GetAllChildren();

	for (int i = 0; i < entries.Num(); i++)
	{
		Cast<UInventoryEntryDisplay>(entries[i])->NativeRefresh(InQuery);
	}
}

bool UInventoryPage::BindHoverDelegate(UObject* InObject, FName InFuncName)
{
	OnEntryHovered.Unbind();

	OnEntryHovered.BindUFunction(InObject, InFuncName);

	return OnEntryHovered.IsBound();
}

bool UInventoryPage::BindClickDelegate(UObject* InObject, FName InFuncName)
{
	OnEntryClicked.Unbind();

	OnEntryClicked.BindUFunction(InObject, InFuncName);

	return OnEntryClicked.IsBound();
}

void UInventoryPage::EntryHoverEvent(UInventoryEntry* InEntry)
{
	OnEntryHovered.ExecuteIfBound(InEntry);
}

void UInventoryPage::EntryClickEvent(UInventoryEntry* InEntry)
{
	OnEntryClicked.ExecuteIfBound(InEntry);
}
