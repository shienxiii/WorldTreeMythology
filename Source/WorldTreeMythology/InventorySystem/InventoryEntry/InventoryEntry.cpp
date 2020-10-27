// Copyright of Maple Game Studio


#include "InventoryEntry.h"

bool UInventoryEntry::InitializeEntry(TSubclassOf<AInventory> InClass, uint8 InCount)
{
    // Only initialize if InventoryClass is NULL
    if (InventoryClass) { return false; }

    InventoryClass = InClass;
    Count = InCount;

    return true;
}

uint8 UInventoryEntry::Add(uint8 InCount)
{
    uint8 remaining = InCount > GetRemainingCapacity() ? InCount - GetRemainingCapacity() : 0;

    Count += InCount - remaining;

    return remaining;
}

void UInventoryEntry::Remove(uint8 InCount)
{
    if (InCount > Count) { Count = 0; }
    else { Count -= InCount; }

    if (Count == 0 && bNullOnEmpty)
    {
        InventoryClass = NULL;
        Count = 0;
    }
}

bool UInventoryEntry::IsChildOf(TSubclassOf<AInventory> InBaseClass)
{
    if (!InventoryClass) { return false; }

    return InventoryClass.Get()->IsChildOf(InBaseClass);
}
