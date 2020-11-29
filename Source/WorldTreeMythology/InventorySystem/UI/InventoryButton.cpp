// Copyright of Maple Game Studio


#include "InventoryButton.h"

void UInventoryButton::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (!EntryButton) { return; }

    EntryButton->OnHovered.AddDynamic(this, &UInventoryButton::EntryHoverEvent);
    EntryButton->OnClicked.AddDynamic(this, &UInventoryButton::EntryClickEvent);
}

FReply UInventoryButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    EntryButton->SetFocus();
    return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UInventoryButton::EntryHoverEvent()
{
    NativeOnEntryHovered.ExecuteIfBound(Entry);
    OnEntryHovered.Broadcast(Entry);
}

void UInventoryButton::EntryClickEvent()
{
    NativeOnEntryClicked.ExecuteIfBound(Entry);
    OnEntryClicked.Broadcast(Entry);
}
