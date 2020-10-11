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

bool UInventoryEntry::IsChildOf(TSubclassOf<AInventory> InBaseClass)
{
    return InventoryClass.Get()->IsChildOf(InBaseClass);
}
