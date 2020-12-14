// Copyright of Maple Game Studio


#include "InventoryPageHorizontal.h"

void UInventoryPageHorizontal::SetupNavigation()
{
    for (int i = 0; i < Entries.Num(); i++)
    {
        Entries[i]->SetNavigationRuleCustom(EUINavigation::Left, NavigationDelegate);
        Entries[i]->SetNavigationRuleCustom(EUINavigation::Right, NavigationDelegate);
    }
}

TSharedRef<SWidget> UInventoryPageHorizontal::RebuildWidget()
{
    Panel = SNew(SInventoryPageHorizontal);

    if (Entries.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Clearing Entries"));
        Entries.Empty();
    }

    for (int i = 0; i < EntryCount && InventoryButtonClass; i++)
    {
        Panel->GetEntries()->AddSlot()
            [
                AddNewButton()->TakeWidget()
            ];
    }

    return Panel.ToSharedRef();
}

void UInventoryPageHorizontal::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    Panel.Reset();
}
