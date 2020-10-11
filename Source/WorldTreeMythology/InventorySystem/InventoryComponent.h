// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryEntry/InventoryList.h"
#include "InventoryComponent.generated.h"

/**
 * The most basic version of the InventoryComponent class.
 * Provides the basic features such as actual list of Inventory entries, adding and removing inventory, query and sort functionalities,
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Determines all the InventoryList that this InventoryComponent will have
	//TArray<TSubclassOf<UInventoryList>> InventoryListType;

	// Array of InventoryList managed by this class. Set which InventoryList will go here on InventoryListType 
	UPROPERTY(BlueprintReadOnly, Category = "Inventory") TArray<UInventoryList*> Inventory;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	void InitializeComponent() override;

	// Call at Construct script to initialize the InventoryList this InventoryComponent will have
	UFUNCTION(BlueprintCallable) void AddInventoryListType(TSubclassOf<UInventoryList> InInventoryList);
	
	/**
	 * Add Inventory to it's respective list. Added Inventory will be in their default base form.
	 * Use AddUniqueToInventory() instead, which will return the InventoryEntry created to add unique traits
	 * 
	 * @return true if successfully added to Inventory
	 */
	UFUNCTION(BlueprintCallable) bool AddToInventory(TSubclassOf<AInventory> InInventory, uint8 InCount = 1);


	/**
	 * Adds a single unique entry for the passed Inventory class and returns a reference to the entry.
	 *
	 * @return Reference to the InventoryEntry class created
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddUniqueToInventory(AInventory* InInventory);

#pragma region InventoryList
	UFUNCTION(BlueprintPure) TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass);
	UFUNCTION(BlueprintPure) TArray<UInventoryEntry*> FilteredQuery(TSubclassOf<AInventory> InInventoryClass, uint8 InQueryFilter);

	UFUNCTION(BlueprintPure) UInventoryList* GetInventoryListFor(TSubclassOf<AInventory> InInventoryClass);
	/**
	 * Gets the index of the InventoryList that can store Inventory of class InInventoryClass
	 * 
	 * @param InInventoryClass The Inventory class to test
	 */
	UFUNCTION(BlueprintPure) int32 IndexOfList(TSubclassOf<AInventory> InInventoryClass);
#pragma endregion
};
