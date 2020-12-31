// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryEntry.h"
#include "InventoryList.generated.h"

/**
 * This class holds an array of InventoryEntry and manages querying to sorting of the items in the array.
 * An InventoryList object is to be initialized inside an InventoryComponent.
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryList : public UObject
{
	GENERATED_BODY()
	
protected:
	// This is the array or list storing all the InventoryEntry
	UPROPERTY(BlueprintReadOnly, Category = "Inventory") TArray<UInventoryEntry*> Inventory;

	// The base class of all Inventory object that this class will store.
	// Only classes that inherited from this base class can be stored
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") TSubclassOf<AInventoryObject> BaseInventoryClass = AInventoryObject::StaticClass();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") TSubclassOf<UInventoryEntry> EntryClass = UInventoryEntry::StaticClass();

	// This defines the number of InventoryEntry to create or remove each time CreateNewEntries() or RemoveEntry() gets called
	UPROPERTY(EditAnywhere, Category = "Inventory") int32 EntryMultiplier = 1;

	/**
	 * Determines whether the copies of the same Inventory class will be grouped in the same entry.
	 * Set to true for Inventory that has unique properties, such as durability or player customization
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") bool bUniqueEntries = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") bool bIsStorage = false;

public:
	/**
	 * Modify the list to fit a specific number of InventoryEntry
	 * If new size is larger than the current size, all the added InventoryEntry will be an empty entry.
	 * If new size is smaller than the current size, the list will be reduced to either the target size or the last non-empty entry
	 */
	UFUNCTION(BlueprintCallable) void ResizeList(int32 InCount);

	/**
	 * Add a new set of InventoryEntry based on the EntryMultiplier
	 * 
	 * @return The first entry of the newly created InventoryEntry
	 */
	UInventoryEntry* CreateNewEntries();

	/**
	 * Remove a set if InventoryEntry based on the EntryMultiplier
	 */
	bool RemoveEntries();

	/**
	 * Adds a single unique entry for the passed Inventory class and returns a pointer to the entry.
	 * 
	 * Does not add anything to the list if bUniqueEntries is false. Use Add() instead in this case.
	 * 
	 * @return Reference to the entry of the added inventory
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddSingle(TSubclassOf<AInventoryObject> InClass);

	/**
	 * Adds InCount number of the passed Inventory class and returns the assigned InventoryEntry if successful.
	 * Does not add anything to the list if bUniqueEntries is true. Use AddUnique() instead in this case.
	 * 
	 * @return Reference to the entry of the added inventory
	 */
	UFUNCTION(BlueprintCallable) int32 AddMultiple(TSubclassOf<AInventoryObject> InClass, int32 InCount = 1);

	/**
	 * Takes a single InventoryObject actor reference and adds it to this InventoryList.
	 * 
	 * @param InActor Reference to the InventoryObject actor to add the this list
	 * 
	 * @return The InventoryEntry that holds the newly added InventoryObject
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) UInventoryEntry* AddByActor(AInventoryObject* InActor);
	virtual UInventoryEntry* AddByActor_Implementation(AInventoryObject* InActor);


	// Blueprint overridable function to add custom condition which determines if it is okay to add to this list
	UFUNCTION(BlueprintNativeEvent) bool CanAddToList();
	virtual bool CanAddToList_Implementation() { return true; }

	// Checks if bUniqueEntries is true
	UFUNCTION(BlueprintCallable) bool IsUniqueEntriesList() { return bUniqueEntries; }

	/**
	 * Returns every InventoryEntry stored on this list, optionally exclude empty entries
	 * 
	 * @param bEmptyEntries When set to true, filters out InventoryEntry that are empty
	 */
	TArray<UInventoryEntry*> QueryForAll(bool bClearEmptyEntries = false);


	//Find and returns the first InventoryEntry that holds the subclass
	UInventoryEntry* GetEntryFor(TSubclassOf<AInventoryObject> InSubclass);

	/**
	 * Queries the list for InventoryObject that are derived from a subclass.
	 * Does not return empty entries.
	 * 
	 * @param InSubclass Subclass of the Inventory to return.
	 */
	TArray<UInventoryEntry*> QueryBySubclass(TSubclassOf<AInventoryObject> InSubclass = NULL);

	/**
	 * BP overriden function to allow custom made query to work seamlessly with InventoryComponent
	 * 
	 * @param InQueryEnum The enum for the custom query in uint8 type
	 */
	UFUNCTION(BlueprintNativeEvent) TArray<UInventoryEntry*> CustomQuery(uint8 InQueryEnum);

	/**
	 * Swaps the position of two InventoryEntry within this InventoryList.
	 * Both InventoryEntry must be stored inside this InventoryList.
	 * 
	 * @param EntryA First entry to swap
	 * @param EntryB Second entry to swap
	 * 
	 * @return Is swap successful?
	 */
	UFUNCTION(BlueprintCallable) bool Swap(UInventoryEntry* EntryA, UInventoryEntry* EntryB);
	
	/**
	 * Gets the base class that all objects in this list should be derived from
	 */
	UFUNCTION(BlueprintPure) TSubclassOf<AInventoryObject> GetBaseInventoryClass() { return BaseInventoryClass; }

	/**
	 * Checks to see if the passed class type can be stored in this InventoryList
	 */
	UFUNCTION(BlueprintCallable) bool CanStore(TSubclassOf<AInventoryObject> InInventoryClass);

	void SetIsStorage(bool InIsStorage) { bIsStorage = InIsStorage; }
};
