// Copyright of Maple Game Studio


#include "InventoryEntry.h"

bool UInventoryEntry::InitializeEntry(TSubclassOf<AInventory> InClass, int32 InCount)
{
    // Only initialize if InventoryClass is NULL
    if (InventoryClass) { return false; }

    InventoryClass = InClass;
    Count = InCount;

    return true;
}

int32 UInventoryEntry::Add(int32 InCount)
{
    int32 remaining = InCount > GetRemainingCapacity() ? InCount - GetRemainingCapacity() : 0;

    Count += InCount - remaining;

    return remaining;
}

void UInventoryEntry::Remove(int32 InCount)
{
    if (InCount > Count) { Count = 0; }
    else { Count -= InCount; }

    if (Count == 0 && bNullOnEmpty)
    {
        InventoryClass = NULL;
        Count = 0;
    }
}

int32 UInventoryEntry::GetRemainingCapacity()
{
    return -1;
}

bool UInventoryEntry::IsChildOf(TSubclassOf<AInventory> InBaseClass)
{
    if (!InventoryClass) { return false; }

    return InventoryClass.Get()->IsChildOf(InBaseClass);
}
