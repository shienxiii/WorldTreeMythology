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
	Inventory[i]->SetIsStorage(bIsStorage);

	return Inventory[i];
}

int32 UInventoryList::Add(TSubclassOf<AInventory> InClass, int32 InCount)
{
	// Test InClass eligibility and whether this list can run this function
	if (!CanStore(InClass) || bUniqueEntries) { return InCount; }

#pragma region FindExistingEntry
	// Find the entry which holds InClass
	int32 i = Inventory.IndexOfByPredicate([InClass](UInventoryEntry* entry)
		{
			return InClass == entry->GetInventoryClass();
		});

	if (i != INDEX_NONE)
	{
		// Entry found, simply add InCount to it
		return Inventory[i]->Add(InCount);
	}
#pragma endregion

#pragma region FindEmptyEntry
	// Find an InventoryEntry with the InventoryClass field set to NULL
	i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	if (i != INDEX_NONE)
	{
		// Found empty InventoryEntry, initialize the Entry and call it's Add
		Inventory[i]->InitializeEntry(InClass, 0);
	}
#pragma endregion
	
	UInventoryEntry* entry = CreateNewEntry();
	entry->InitializeEntry(InClass, 0);

	return entry->Add(InCount);
}

UInventoryEntry* UInventoryList::AddUnique(TSubclassOf<AInventory> InClass)
{
	// Test InClass eligibility
	if (!CanStore(InClass) || !bUniqueEntries) { return nullptr; }

	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	UInventoryEntry* entry = i != INDEX_NONE ? Inventory[i] : CreateNewEntry();

	entry->InitializeEntry(InClass);

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
