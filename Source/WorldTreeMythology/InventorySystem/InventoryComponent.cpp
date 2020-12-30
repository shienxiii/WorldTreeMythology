// Copyright of Maple Game Studio


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UInventoryComponent::AddInventoryListType(TSubclassOf<UInventoryList> InInventoryList)
{
	if (!InInventoryList) { return; }

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

int32 UInventoryComponent::AddBySubclass(TSubclassOf<AInventoryObject> InInventory, int32 InCount)
{
	UInventoryList* list = GetInventoryListFor(InInventory);

	if (!list) { return InCount; }

	return list->AddMultiple(InInventory, InCount);
}

UInventoryEntry* UInventoryComponent::AddByActor(AInventoryObject* InInventory, bool bDestroyActor)
{
	UInventoryList* list = GetInventoryListFor(InInventory->GetClass());

	if (!list) { return nullptr; }

	UInventoryEntry* entry = list->AddByActor(InInventory);

	if (bDestroyActor) { InInventory->Destroy(); }

	return entry;
}

TArray<UInventoryEntry*> UInventoryComponent::QueryByBaseClass(TSubclassOf<AInventoryObject> InInventoryClass, bool bClearEmptyEntries)
{
	TArray<UInventoryList*> lists = Inventory.FilterByPredicate([InInventoryClass](UInventoryList* list)
		{
			return list->GetBaseInventoryClass().Get()->IsChildOf(InInventoryClass);
		}
	);

	TArray<UInventoryEntry*> entries;

	for (int i = 0; i < lists.Num(); i++)
	{
		entries += lists[i]->QueryForAll(bClearEmptyEntries);
	}

	return entries;
}

TArray<UInventoryEntry*> UInventoryComponent::QueryBySubclass(TSubclassOf<AInventoryObject> InSubclass)
{
	UInventoryList* list = GetInventoryListFor(InSubclass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->QueryBySubclass(InSubclass);
}

TArray<UInventoryEntry*> UInventoryComponent::CustomQuery(TSubclassOf<AInventoryObject> InInventoryClass, uint8 InQueryEnum)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->CustomQuery(InQueryEnum);
}

UInventoryList* UInventoryComponent::GetInventoryListFor(TSubclassOf<AInventoryObject> InInventoryClass)
{
	UInventoryList** listRef = Inventory.FindByPredicate([InInventoryClass](UInventoryList* list)
		{
			return list->CanStore(InInventoryClass);
		});

	return listRef ? *listRef : nullptr;
}
