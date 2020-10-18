// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryEntry.h"
#include "InventoryList.generated.h"

/**
 * This class holds an array of InventoryEntry and manages querying to sorting of the items in the array.
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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") TSubclassOf<AInventory> BaseInventoryClass;

	/**
	 * Determines whether the copies of the same Inventory class will be grouped in the same entry.
	 * Set to true for Inventory that has unique properties, such as durability or player customization
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") bool bUniqueEntries = false;

public:
	/**
	 * Adds InCount number of the passed Inventory class and returns true if successful.
	 * Inventory objects are added in their default state. To store unique traits of each Inventory object, use AddUnique()
	 */
	UFUNCTION(BlueprintCallable) bool Add(TSubclassOf<AInventory> InClass, uint8 InCount = 1);

	/**
	 * Adds a single unique entry for the passed Inventory class and returns a pointer to the entry.
	 * 
	 * NOTE TO SELF: make BlueprintNativeEvent
	 * bUniqueEntries must be true
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddUnique(TSubclassOf<AInventory> InClass);

	// Checks if this bUniqueEntries is true
	UFUNCTION(BlueprintCallable) bool IsUniqueEntriesList() { return bUniqueEntries; }


	/**
	 * Queries the list for Inventory that are derived from a subclass, which is derived from BaseClass
	 * 
	 * @param InSubclass Subclass of the Inventory to return, must be derived from BaseClass
	 */
	TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass = NULL);

	/**
	 * BP overriden function to allow custom made query to work seamlessly with InventoryComponent
	 * 
	 * @param InQueryEnum The enum for the custom query in uint8 type
	 */
	UFUNCTION(BlueprintNativeEvent) TArray<UInventoryEntry*> CustomQuery(uint8 InQueryEnum);
	
	/**
	 * Gets the base class that all objects in this list should be derived from
	 */
	UFUNCTION(BlueprintPure) TSubclassOf<AInventory> GetBaseInventoryClass() { return BaseInventoryClass; }


	/**
	 * Checks to see if the passed class type can be stored in this InventoryList
	 */
	UFUNCTION(BlueprintCallable) bool CanStore(TSubclassOf<AInventory> InInventoryClass);
};
