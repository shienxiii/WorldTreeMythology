// Copyright of Maple Game Studio


#include "InventoryPage.h"


UInventoryPage::UInventoryPage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NavigationDelegate.BindUFunction(this, TEXT("NavigatePanel"));
}

void UInventoryPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetupNavigation();
}

void UInventoryPage::SetIndex(int32 InIndex)
{
	Index = InIndex;

	// Set the index of the InventoryButtons in Entries
	int startIndex = GetEntryCount() * Index;

	for (int i = 0; i < Entries.Num(); i++)
	{
		Entries[i]->SetIndex(startIndex + i);
	}
}

void UInventoryPage::RefreshQuery(TArray<UInventoryEntry*> InEntry)
{
	for (UInventoryButton* entry : Entries)
	{
		entry->RefreshQuery(InEntry);
	}
}

int32 UInventoryPage::GetEntryIndex(UInventoryButton* InButton)
{
	if (!InButton) { return INDEX_NONE; }

	int32 index = Entries.IndexOfByPredicate([InButton](const UWidget* widget)
		{
			return widget == InButton;
		});

	return index;
}

UInventoryButton* UInventoryPage::GetEntryAt(int32 InIndex)
{
	if (InIndex >= Entries.Num() || InIndex < 0) { return nullptr; }

	return Entries[InIndex];
}

UInventoryButton* UInventoryPage::AddNewButton()
{
	if (!InventoryButtonClass) { return nullptr; }

	int i = Entries.Add(CreateWidget<UInventoryButton>(GetWorld(), InventoryButtonClass));

	// Bind Callback Event
	Entries[i]->BindNativeOnEntryHovered(this, "NativeEntryHoverEvent");
	Entries[i]->BindNativeOnEntryClicked(this, "NativeEntryClickEvent");
	Entries[i]->BindOnAddedToFocus(this, "SetFocusedEntry");
	Entries[i]->SetIndex((GetEntryCount() * Index) + i);

	return Entries[i];
}

FReply UInventoryPage::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    if (Entries.Num() == 0) { return FReply::Unhandled(); }

    if (FocusedEntry)
	{
		FocusedEntry->SetFocus();
	}
    else if(Entries[0]->bIsEnabled && Entries[0]->IsVisible())
	{
		Entries[0]->SetFocus();
	}

    return FReply::Handled();
}

UWidget* UInventoryPage::NavigatePanel(EUINavigation InNavigation)
{
	if (!FocusedEntry) { return nullptr; }

	int32 index = GetEntryIndex(FocusedEntry);
	if (index == INDEX_NONE) { return nullptr; }

	// circular navigation for now
	if (InNavigation == EUINavigation::Down || InNavigation == EUINavigation::Right)
	{
		index++;
		index %= Entries.Num();

		if (Entries[index]->IsNullEntry()) { index = 0; }
	}
	else
	{
		do
		{
			index--;
			index = index > -1 ? index : Entries.Num() - 1;
		} while (Entries[index]->IsNullEntry());
	}

	return Entries[index];
}

void UInventoryPage::NativeEntryHoverEvent(UInventoryEntry* InEntry)
{
	NativeOnEntryHovered.ExecuteIfBound(InEntry);
	OnEntryHovered.Broadcast(InEntry);
}

void UInventoryPage::NativeEntryClickEvent(UInventoryEntry* InEntry)
{
	NativeOnEntryClicked.ExecuteIfBound(InEntry);
	OnEntryClicked.Broadcast(InEntry);
}
