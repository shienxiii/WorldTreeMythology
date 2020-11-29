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

bool UInventoryComponent::AddBySubclass(TSubclassOf<AInventory> InInventory, int32 InCount)
{
	UInventoryList* list = GetInventoryListFor(InInventory);

	if (!list) { return false; }

	list->Add(InInventory, InCount);

	return true;
}

UInventoryEntry* UInventoryComponent::AddByActor(AInventory* InInventory)
{
	UInventoryList* list = GetInventoryListFor(InInventory->GetClass());

	if (!list) { return nullptr; }

	if (!list->IsUniqueEntriesList())
	{
		AddBySubclass(InInventory->StaticClass(), 1);
		return nullptr;
	}

	return list->AddUnique(InInventory->GetClass());
}

TArray<UInventoryEntry*> UInventoryComponent::QueryForSubclass(TSubclassOf<AInventory> InSubclass)
{
	UInventoryList* list = GetInventoryListFor(InSubclass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->QueryForSubclass(InSubclass);
}

TArray<UInventoryEntry*> UInventoryComponent::CustomQuery(TSubclassOf<AInventory> InInventoryClass, uint8 InQueryEnum)
{
	UInventoryList* list = GetInventoryListFor(InInventoryClass);

	if (!list) { return TArray<UInventoryEntry*>(); }

	return list->CustomQuery(InQueryEnum);
}

UInventoryList* UInventoryComponent::GetInventoryListFor(TSubclassOf<AInventory> InInventoryClass)
{
	int32 i = IndexOfList(InInventoryClass);

	if (i == INDEX_NONE) { return nullptr; }

	return Inventory[i];
}

int32 UInventoryComponent::IndexOfList(TSubclassOf<AInventory> InInventoryClass)
{
	return Inventory.IndexOfByPredicate([InInventoryClass](UInventoryList* List)
		{
			return List->CanStore(InInventoryClass);
		});
}
