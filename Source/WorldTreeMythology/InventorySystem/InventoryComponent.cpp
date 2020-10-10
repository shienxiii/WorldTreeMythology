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

	// Initialize all the InventoryList set for this InventoryComponent
	for (TSubclassOf<UInventoryList> list : InventoryListType)
	{
		Inventory.Add(NewObject<UInventoryList>((UObject*)GetTransientPackage(), list));
	}
}

TArray<UInventoryEntry*> UInventoryComponent::QueryForSubclass(TSubclassOf<AInventory> InSubclass)
{
	int32 i = IndexOfList(InSubclass);

	if (i == INDEX_NONE) { return TArray<UInventoryEntry*>(); }

	return Inventory[i]->QueryForSubclass(InSubclass);
}

TArray<UInventoryEntry*> UInventoryComponent::CustomQuery(TSubclassOf<AInventory> InInventoryClass, uint8 InQueryType)
{
	int32 i = IndexOfList(InInventoryClass);

	if (i == INDEX_NONE) { return TArray<UInventoryEntry*>(); }

	return Inventory[i]->CustomQuery(InQueryType);
}

int32 UInventoryComponent::IndexOfList(TSubclassOf<AInventory> InInventoryClass)
{
	return Inventory.IndexOfByPredicate([InInventoryClass](UInventoryList* List)
		{
			return List->CanStore(InInventoryClass);
		});
}
