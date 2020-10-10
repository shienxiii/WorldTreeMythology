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
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") TArray<UInventoryEntry*> Inventory;

	// This field defines the base class of the managed Inventory class, Add() will test against this class to determine whether to accept or reject the incoming Inventory object
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") TSubclassOf<AInventory> BaseClass;

	/**
	 * Determines whether the copies of the same Inventory class will be grouped in the same entry.
	 * Set to true for Inventory that has unique properties, such as durability or player customization
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") bool bUniqueEntries = false;

public:
	UFUNCTION() bool Add(TSubclassOf<AInventory> InClass, uint8 InCount = 1);

	/**
	 * Queries the list for Inventory that are derived from a subclass, which is derived from BaseClass
	 * 
	 * @param InSubclass Subclass of the Inventory to return, must be derived from BaseClass
	 */
	TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass = NULL);

	
	UFUNCTION(BlueprintImplementableEvent) TArray<UInventoryEntry*> CustomQuery(uint8 InQueryEnum);

	UFUNCTION() TSubclassOf<AInventory> GetBaseClass() { return BaseClass; }
	bool CanStore(TSubclassOf<AInventory> InInventoryClass);
};
