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
	if (!CanStore(InClass) || bUniqueEntries || !CanAddToList()) { return InCount; }

#pragma region FindExistingEntry
	// Find the entry which holds InClass
	if(UInventoryEntry* entry = GetEntryFor(InClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding to existing entry"));
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
		UE_LOG(LogTemp, Warning, TEXT("Adding to empty slot"));
		// Found empty InventoryEntry, initialize the Entry and call it's Add
		Inventory[i]->InitializeEntry(InClass, 0);
	}
#pragma endregion
	
	UE_LOG(LogTemp, Warning, TEXT("creating new entry"));
	UInventoryEntry* entry = CreateNewEntry();
	entry->InitializeEntry(InClass, 0);

	return entry->Add(InCount);
}

UInventoryEntry* UInventoryList::AddUnique(TSubclassOf<AInventory> InClass)
{
	// Test InClass eligibility
	if (!CanStore(InClass) || !bUniqueEntries || !CanAddToList()) { return nullptr; }

	// Find an InventoryEntry with the InventoryClass field set to NULL
	int32 i = Inventory.IndexOfByPredicate([](UInventoryEntry* entry)
		{
			return entry->GetInventoryClass() == NULL;
		});

	if (i == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding to empty slot"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("creating new entry"));
	}

	UInventoryEntry* entry = i != INDEX_NONE ? Inventory[i] : CreateNewEntry();

	entry->InitializeEntry(InClass);

	return entry;
}

UInventoryEntry* UInventoryList::GetEntryFor(TSubclassOf<AInventory> InSubclass)
{
	UInventoryEntry** entry = Inventory.FindByPredicate([InSubclass](UInventoryEntry* entry)
								{
									return entry->GetInventoryClass() == InSubclass;
								}
	);

	return entry ? *entry : nullptr;
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
