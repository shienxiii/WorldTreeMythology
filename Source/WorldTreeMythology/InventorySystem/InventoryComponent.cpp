// Copyright of Maple Game Studio


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	for (TSubclassOf<UInventoryList> list : InventoryLists)
	{
		AddInventoryListType(list);
	}
}

void UInventoryComponent::AddInventoryListType(TSubclassOf<UInventoryList> InInventoryList)
{
	if (!InInventoryList) { return; }

	UInventoryList** duplicate = Inventory.FindByPredicate([InInventoryList](UInventoryList* list)
		{
			return list->IsA(InInventoryList);
		});

	int32 i = Inventory.Add(NewObject<UInventoryList>((UObject*)GetTransientPackage(), InInventoryList));
	Inventory[i]->SetIsStorage(bIsStorage);
}

void UInventoryComponent::SetIsStorage(bool InIsStorage)
{
	for (UInventoryList* list : Inventory)
	{
		list->SetIsStorage(bIsStorage);
	}
}

int32 UInventoryComponent::AddBySubclass(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass);

	if (!list) { return InCount; }

	return list->AddMultiple(InInventoryClass, InCount);
}

UInventoryEntry* UInventoryComponent::AddByActor(AInventoryObject* InInventoryClass, bool bDestroyActor)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass->GetClass());

	if (!list) { return nullptr; }

	UInventoryEntry* entry = list->AddByActor(InInventoryClass);

	if (bDestroyActor) { InInventoryClass->Destroy(); }

	return entry;
}

TArray<UInventoryEntry*> UInventoryComponent::QueryByBaseClass(TSubclassOf<AInventoryObject> InInventoryClass, bool bClearEmptyEntries)
{
	TArray<UInventoryList*> lists = GetInventoryListsFor(InInventoryClass);

	TArray<UInventoryEntry*> entries;

	for (int i = 0; i < lists.Num(); i++)
	{
		entries += lists[i]->QueryForAll(bClearEmptyEntries);
	}

	return entries;
}

TArray<UInventoryEntry*> UInventoryComponent::QueryBySubclass(TSubclassOf<AInventoryObject> InInventoryClass)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->QueryBySubclass(InInventoryClass);
}

TArray<UInventoryEntry*> UInventoryComponent::CustomQuery(TSubclassOf<AInventoryObject> InInventoryClass, uint8 InQueryEnum)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->CustomQuery(InQueryEnum);
}

int32 UInventoryComponent::CountInventory(TSubclassOf<AInventoryObject> InInventoryClass)
{
	TArray<UInventoryList*> lists = GetInventoryListsFor(InInventoryClass);

	if (lists.Num() == 0) { return INDEX_NONE; }

	int32 count = 0;

	for (UInventoryList* list : lists)
	{

		TArray<UInventoryEntry*> entries = list->QueryBySubclass(InInventoryClass);

		if (list->IsUniqueEntriesList())
		{
			count += entries.Num();
		}
		else
		{
			for (UInventoryEntry* entry : entries)
			{
				count += entry->GetCount();
			}
		}
	}
	return count;
}

UInventoryList* UInventoryComponent::GetInventoryListFor(TSubclassOf<AInventoryObject> InInventoryClass)
{
	UInventoryList** listRef = Inventory.FindByPredicate([InInventoryClass](UInventoryList* list)
		{
			return list->CanStore(InInventoryClass);
		});

	return listRef ? *listRef : nullptr;
}

TArray<UInventoryList*> UInventoryComponent::GetInventoryListsFor(TSubclassOf<AInventoryObject> InInventoryClass)
{
	return Inventory.FilterByPredicate([InInventoryClass](UInventoryList* list)
		{
			return list->GetBaseInventoryClass().Get()->IsChildOf(InInventoryClass);
		}
	);
}
