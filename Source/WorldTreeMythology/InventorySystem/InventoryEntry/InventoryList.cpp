// Copyright of Maple Game Studio


#include "InventoryList.h"

bool UInventoryList::Add(TSubclassOf<AInventory> InClass, uint8 InCount)
{
	// Test InClass eligibility
	if(!CanStore(InClass))
	{
		return false;
	}

	// If this list should hold unique entry, simply create a new entry
	if (bUniqueEntries)
	{
		// Create new entry
		int32 i = Inventory.Add(NewObject<UInventoryEntry>());
		Inventory[i]->InitializeEntry(InClass, InCount);

		return true;
	}

	// Find the entry which holds InClass
	int32 i = Inventory.IndexOfByPredicate([InClass](UInventoryEntry* entry)
		{
			return InClass == entry->GetInventoryClass();
		});


	if (i == INDEX_NONE)
	{
		// Create new entry if none is found
		i = Inventory.Add(NewObject<UInventoryEntry>());
		Inventory[i]->InitializeEntry(InClass, InCount);
	}
	else
	{
		// Otherwise simple add InCount to the entry
		Inventory[i]->Add(InCount);
	}

	return true;
}

TArray<UInventoryEntry*> UInventoryList::QueryForSubclass(TSubclassOf<AInventory> InSubclass)
{
	if (InSubclass == NULL)
	{
		InSubclass = BaseClass;
	}
	else if (!CanStore(InSubclass))
	{
		return TArray<UInventoryEntry*>();
	}

	return Inventory.FilterByPredicate([InSubclass](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass().Get()->IsChildOf(InSubclass);
		});
}

bool UInventoryList::CanStore(TSubclassOf<AInventory> InInventoryClass)
{
	if (InInventoryClass == BaseClass) { return true; }

	return InInventoryClass.Get()->IsChildOf(BaseClass);
}
