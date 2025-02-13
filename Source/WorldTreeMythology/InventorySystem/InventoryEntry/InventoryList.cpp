// Copyright of Maple Game Studio


#include "InventoryList.h"

void UInventoryList::ResizeList(int32 InCount)
{
	if (Inventory.Num() < InCount)
	{
		while (Inventory.Num() < InCount)
		{
			CreateNewEntries();
		}
	}
	else
	{
		while (Inventory.Num() > InCount && 
			Inventory.Num() - (EntryMultiplier - 1) >= InCount &&
			(RemoveEntries()))
		{
			// Loop until false
		}
	}
}

UInventoryEntry* UInventoryList::CreateNewEntries()
{
	int index = Inventory.Num();

	for (int i = 0; i < EntryMultiplier; i++)
	{
		int n = Inventory.Add(NewObject<UInventoryEntry>(this, EntryClass));
		Inventory[n]->SetIsStorage(bIsStorage);
	}

	return Inventory[index];
}

bool UInventoryList::RemoveEntries()
{
	bool bCanRemove = true;

	// Get the last index of the array
	int n = Inventory.Num() - 1;

	// Check if can remove the the last set of entries based on the EntryMultiplier property
	for (int i = 0; i < EntryMultiplier && bCanRemove; i++)
	{
		if (!Inventory[n - i] || !(Inventory[n - i]->IsEmptyEntry())) { bCanRemove = false; }
	}

	if (!bCanRemove) { return false; }
	
	Inventory.RemoveAt(n - (EntryMultiplier - 1), EntryMultiplier, true);

	return true;
}

UInventoryEntry* UInventoryList::AddSingle(TSubclassOf<AInventoryObject> InInventoryClass)
{
	// Test InInventoryClass eligibility
	if (!CanStore(InInventoryClass) || !CanAddToList()) { return nullptr; }

#pragma region FindExistingEntry
	if (!bUniqueEntries)
	{
		if (UInventoryEntry* entry = GetEntryFor(InInventoryClass))
		{
			return entry->Add(1) == 0 ? entry : nullptr;
		}
	}
#pragma endregion

	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	UInventoryEntry* entry = i != INDEX_NONE ? Inventory[i] : CreateNewEntries();

	entry->InitializeEntry(InInventoryClass);

	return entry;
}

int32 UInventoryList::AddMultiple(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount)
{
	// Test InInventoryClass eligibility and whether this list can run this function
	if (!CanStore(InInventoryClass) || !CanAddToList()) { return InCount; }


	if (bUniqueEntries)
	{
		int32 excess = InCount;

		while (excess > 0 && AddSingle(InInventoryClass))
		{
			excess--;
		}

		return excess;
	}

#pragma region FindExistingEntry
	// Find the entry which holds InInventoryClass
	if(UInventoryEntry* entry = GetEntryFor(InInventoryClass))
	{
		// Entry found, simply add InCount to it
		return entry->Add(InCount);
	}
#pragma endregion

#pragma region FindEmptyEntry
	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	if (i != INDEX_NONE)
	{
		// Found empty InventoryEntry, initialize the Entry and call it's Add
		Inventory[i]->InitializeEntry(InInventoryClass, 0);
	}
#pragma endregion

	UInventoryEntry* entry = CreateNewEntries();
	entry->InitializeEntry(InInventoryClass, 0);

	return entry->Add(InCount);
}

UInventoryEntry* UInventoryList::AddByActor_Implementation(AInventoryObject* InActor)
{
	UInventoryEntry* entry = AddSingle(InActor->GetClass());

	return entry;
}


TArray<UInventoryEntry*> UInventoryList::QueryForAll(bool bClearEmptyEntries)
{
	if(!bClearEmptyEntries) { return Inventory; }

	return Inventory.FilterByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() != NULL;
		});
}

UInventoryEntry* UInventoryList::GetEntryFor(TSubclassOf<AInventoryObject> InInventoryClass)
{
	UInventoryEntry** entry = Inventory.FindByPredicate([InInventoryClass](UInventoryEntry* entry)
								{
									return entry->GetInventoryClass() == InInventoryClass;
								}
	);

	return entry ? *entry : nullptr;
}

TArray<UInventoryEntry*> UInventoryList::QueryBySubclass(TSubclassOf<AInventoryObject> InInventoryClass)
{
	if (InInventoryClass == NULL)
	{
		InInventoryClass = BaseInventoryClass;
	}
	else if (!CanStore(InInventoryClass))
	{
		return TArray<UInventoryEntry*>();
	}


	return Inventory.FilterByPredicate([InInventoryClass](UInventoryEntry* entry)
		{
			return entry->IsChildOf(InInventoryClass);
		});
}

TArray<UInventoryEntry*> UInventoryList::CustomQuery_Implementation(uint8 InQueryEnum)
{
	return QueryBySubclass();
}

bool UInventoryList::Swap(UInventoryEntry* EntryA, UInventoryEntry* EntryB)
{
	int a = Inventory.IndexOfByPredicate([EntryA](UInventoryEntry* entry)
		{
			return entry == EntryA;
		});

	int b = Inventory.IndexOfByPredicate([EntryB](UInventoryEntry* entry)
		{
			return entry == EntryB;
		});

	if (a == INDEX_NONE || b == INDEX_NONE) { return false; }

	Inventory.Swap(a, b);
	return true;
}

bool UInventoryList::CanStore(TSubclassOf<AInventoryObject> InInventoryClass)
{
	return InInventoryClass.Get()->IsChildOf(BaseInventoryClass);
}
