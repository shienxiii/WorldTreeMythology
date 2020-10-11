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
	UPROPERTY(BlueprintReadOnly, Category = "Inventory") TArray<UInventoryEntry*> Inventory;

	// This field defines the base class of the managed Inventory class, Add() will test against this class to determine whether to accept or reject the incoming Inventory object
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
	 * bUniqueEntries must be true
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddUnique(TSubclassOf<AInventory> InClass);

	/**
	 * Queries the list for Inventory that are derived from a subclass, which is derived from BaseClass
	 * 
	 * @param InSubclass Subclass of the Inventory to return, must be derived from BaseClass
	 */
	TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass = NULL);

	UFUNCTION(BlueprintNativeEvent) TArray<UInventoryEntry*> CustomQuery(uint8 InQueryEnum);
	
	UFUNCTION(BlueprintPure) TSubclassOf<AInventory> GetBaseInventoryClass() { return BaseInventoryClass; }

	bool CanStore(TSubclassOf<AInventory> InInventoryClass);
};
