// Copyright of Maple Game Studio


#include "InventoryList.h"

UInventoryEntry* UInventoryList::Add(TSubclassOf<AInventory> InClass, uint8 InCount)
{
	// Test InClass eligibility
	if (!CanStore(InClass)) { return nullptr; }

	// If this list should hold unique entry, add using AddUnique() and return the last created entry
	if (bUniqueEntries)
	{
		UInventoryEntry* entry = nullptr;

		for (int i = 0; i < InCount; i++)
		{
			entry = AddUnique(InClass);
		}

		return entry;
	}

	// Find the entry which holds InClass
	int32 i = Inventory.IndexOfByPredicate([InClass](UInventoryEntry* entry)
		{
			return InClass == entry->GetInventoryClass();
		});

	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 n = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});


	if (i != INDEX_NONE)
	{
		// Entry found, simply add InCount to it
		Inventory[i]->Add(InCount);
	}
	else if (n != INDEX_NONE)
	{
		// Found empty InventoryEntry, set i to n and initialize the Entry
		i = n;

		Inventory[i]->InitializeEntry(InClass, InCount);
	}
	else
	{
		// Create new entry if both query returned nothing
		i = Inventory.Add(NewObject<UInventoryEntry>(this, EntryClass));

		Inventory[i]->InitializeEntry(InClass, InCount);
	}

	return Inventory[i];
}

UInventoryEntry* UInventoryList::AddUnique(TSubclassOf<AInventory> InClass)
{
	// Test InClass eligibility
	if (!CanStore(InClass)) { return nullptr; }

	if (!bUniqueEntries)
	{
		Add(InClass, 1);
		int32 i = Inventory.IndexOfByPredicate([InClass](UInventoryEntry* entry)
			{
				return InClass == entry->GetInventoryClass();
			});

		return Inventory[i];
	}

	int32 i = Inventory.Add(NewObject<UInventoryEntry>(this, EntryClass));

	Inventory[i]->InitializeEntry(InClass, 1);

	return Inventory[i];
}

TArray<UInventoryEntry*> UInventoryList::QueryForSubclass(TSubclassOf<AInventory> InSubclass)
{
	if (InSubclass == NULL)
	{
		InSubclass = BaseInventoryClass;
	}
	else if (!CanStore(InSubclass))
	{
		return TArray<UInventoryEntry*>();
	}


	return Inventory.FilterByPredicate([InSubclass](UInventoryEntry* entry)
		{
			return entry->IsChildOf(InSubclass);
		});
}

TArray<UInventoryEntry*> UInventoryList::CustomQuery_Implementation(uint8 InQueryEnum)
{
	return QueryForSubclass();
}

bool UInventoryList::CanStore(TSubclassOf<AInventory> InInventoryClass)
{
	return InInventoryClass.Get()->IsChildOf(BaseInventoryClass);
}
