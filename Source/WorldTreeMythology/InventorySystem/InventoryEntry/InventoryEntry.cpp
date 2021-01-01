// Copyright of Maple Game Studio


#include "InventoryEntry.h"

bool UInventoryEntry::InitializeEntry_Implementation(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount)
{
    // Only initialize if InventoryClass is NULL
    if (InventoryClass) { return false; }

    InventoryClass = InInventoryClass;
    Count = InCount;

    return true;
}

int32 UInventoryEntry::Add(int32 InCount)
{
    // Find if there will be excess object after adding
    int32 excess = InCount > GetRemainingCount() ? InCount - GetRemainingCount() : 0;

    Count += (InCount - excess);

    return excess;
}

int32 UInventoryEntry::Remove(int32 InCount)
{
    if (InCount > Count) { Count = 0; }
    else { Count -= InCount; }

    if (Count == 0 && bClearOnEmpty)
    {
        ClearEntry();
    }

    return Count;
}

void UInventoryEntry::ClearEntry_Implementation()
{
    InventoryClass = NULL;
    Count = 0;
}

int32 UInventoryEntry::GetRemainingCount()
{
    return GetMaxCount() - Count;
}

int32 UInventoryEntry::GetMaxCount()
{
    if (!bIsStorage)
    {
        return GetInventoryDefault()->GetMaxCarry();
    }
    else
    {
        return GetInventoryDefault()->GetMaxStorage();;
    }
}

bool UInventoryEntry::IsChildOf(TSubclassOf<AInventoryObject> InBaseClass)
{
    if (!InventoryClass) { return false; }

    return InventoryClass.Get()->IsChildOf(InBaseClass);
}
