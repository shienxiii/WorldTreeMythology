// Copyright of Maple Game Studio


#include "InventoryList.h"

void UInventoryList::MofifyListSize(int32 InCount)
{
	if (Inventory.Num() < InCount)
	{
		while (Inventory.Num() < InCount)
		{
			CreateNewEntry();
		}
	}
	else
	{
		while (Inventory.Num() > InCount && 
			(Inventory[Inventory.Num() - 1]->GetInventoryClass()) == NULL)
		{
			Inventory.RemoveAt(Inventory.Num() - 1, 1, true);
		}
	}
}

UInventoryEntry* UInventoryList::CreateNewEntry()
{
	int i = Inventory.Add(NewObject<UInventoryEntry>(this, EntryClass));

	return Inventory[i];
}

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
		CreateNewEntry()->InitializeEntry(InClass, InCount);
		i = Inventory.Num() - 1;
	}

	return Inventory[i];
}

UInventoryEntry* UInventoryList::AddUnique(TSubclassOf<AInventory> InClass)
{
	// Test InClass eligibility
	if (!CanStore(InClass)) { return nullptr; }

	if (!bUniqueEntries) { return Add(InClass, 1); }

	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	UInventoryEntry* entry = i != INDEX_NONE ? Inventory[i] : CreateNewEntry();

	entry->InitializeEntry(InClass, 1);

	return entry;
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
