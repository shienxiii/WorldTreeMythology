// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../InventoryClass/Inventory.h"
#include "InventoryEntry.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryEntry : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly) TSubclassOf<AInventory> InventoryClass = NULL;
	UPROPERTY(BlueprintReadOnly) uint8 Count = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere) uint8 MaxCount = 255;

	// Whether to set InventoryClass to NULL when Count == 0 on Remove()
	UPROPERTY(BlueprintReadOnly, EditAnywhere) bool bNullOnEmpty = true;

public:
	/**
	 * Initializes the InventoryClass and Count variable if InventoryClass is NULL
	 */
	bool InitializeEntry(TSubclassOf<AInventory> InClass, uint8 InCount);

	/**
	 * Attempt to increment InCount number of item to Count and return the number of item remaining if capacity is reached
	 * 
	 * @param InCount The increment to Count
	 * 
	 * @return remainder of item not added to Count
	 */
	uint8 Add(uint8 InCount);

	/**
	 * Remove the InCount number of item from Count.
	 * If Count reaches 0 and bNullOnEmpty is true, nullify this entry
	 */
	void Remove(uint8 InCount);



	TSubclassOf<AInventory> GetInventoryClass() { return InventoryClass; }
	uint8 GetCount() { return Count; }

	// Gets the remaining increment that can be added to this InventoryEntry
	UFUNCTION(BlueprintPure) uint8 GetRemainingCapacity() { return MaxCount - Count; }

	/** 
	 * Check if the InventoryClass stored by this entry is a child of a base class
	 * 
	 * @param InBaseClass The base class to test
	 */
	bool IsChildOf(TSubclassOf<AInventory> InBaseClass);

	UFUNCTION(BlueprintPure) AInventory* GetInventoryDefault() { return InventoryClass.GetDefaultObject(); }
};
