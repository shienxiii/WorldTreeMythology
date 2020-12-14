// Copyright of Maple Game Studio


#include "InventoryButton.h"

void UInventoryButton::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!EntryButton) { return; }

    EntryButton->OnHovered.AddDynamic(this, &UInventoryButton::EntryHoverEvent);
    EntryButton->OnClicked.AddDynamic(this, &UInventoryButton::EntryClickEvent);

    NormalMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(EntryButton->WidgetStyle.Normal.GetResourceObject()), nullptr);
    HoverMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(EntryButton->WidgetStyle.Hovered.GetResourceObject()), nullptr);
    ClickMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(EntryButton->WidgetStyle.Pressed.GetResourceObject()), nullptr);
}

void UInventoryButton::RefreshQuery(TArray<UInventoryEntry*> InEntry)
{
    if (Index >= InEntry.Num())
    {
        Entry = nullptr;

        if (NullState == EEntryNullState::HIDE)
        {
            SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            SetIsEnabled(false);
            EntryButton->SetIsEnabled(false);
        }

        return;
    }

    Entry = InEntry[Index];

    SetIsEnabled(true);
    EntryButton->SetIsEnabled(true);

    if (!Entry->GetInventoryDefault()) { return; }

    NormalMatDynamic->SetTextureParameterValue(IconParameter, Entry->GetInventoryDefault()->GetIcon());
    HoverMatDynamic->SetTextureParameterValue(IconParameter, Entry->GetInventoryDefault()->GetIcon());
    ClickMatDynamic->SetTextureParameterValue(IconParameter, Entry->GetInventoryDefault()->GetIcon());

    EntryButton->WidgetStyle.Normal.SetResourceObject(NormalMatDynamic);
    EntryButton->WidgetStyle.Hovered.SetResourceObject(HoverMatDynamic);
    EntryButton->WidgetStyle.Pressed.SetResourceObject(ClickMatDynamic);
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
