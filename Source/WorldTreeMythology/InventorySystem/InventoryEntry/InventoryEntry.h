// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../InventoryClass/Inventory.h"
#include "InventoryEntry.generated.h"

/**
 * This class is used by InventoryList to keep track of an instance of Inventory object owned by an InventoryComponent.
 */
UCLASS(Blueprintable, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryEntry : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly) TSubclassOf<AInventory> InventoryClass = NULL;
	UPROPERTY(BlueprintReadOnly) int32 Count = 0;

	// Whether to set InventoryClass to NULL when Count == 0 on Remove()
	UPROPERTY(BlueprintReadOnly, EditAnywhere) bool bNullOnEmpty = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) bool bIsStorage = false;

public:
	/**
	 * Initializes the InventoryClass and Count variable if InventoryClass is NULL
	 */
	UFUNCTION(BlueprintNativeEvent) bool InitializeEntry(TSubclassOf<AInventory> InClass, int32 InCount);
	

	/**
	 * Attempt to increment InCount number of item to Count and return the number of item remaining if capacity is reached
	 * 
	 * @param InCount The increment to Count
	 * 
	 * @return remainder of item not added to Count
	 */
	UFUNCTION(BlueprintCallable) int32 Add(int32 InCount = 1);

	/**
	 * Remove the InCount number of item from Count.
	 * If Count reaches 0 and bNullOnEmpty is true, nullify this entry
	 */
	UFUNCTION(BlueprintCallable) void Remove(int32 InCount = 1);

	TSubclassOf<AInventory> GetInventoryClass() { return InventoryClass; }
	int32 GetCount() { return Count; }

	// Gets the remaining increment that can be added to this InventoryEntry
	UFUNCTION(BlueprintPure) int32 GetRemainingCount();

	UFUNCTION(BlueprintPure) int32 GetMaxCount();

	/** 
	 * Check if the InventoryClass stored by this entry is a child of a base class
	 * 
	 * @param InBaseClass The base class to test
	 */
	UFUNCTION(BlueprintPure) bool IsChildOf(TSubclassOf<AInventory> InBaseClass);

	UFUNCTION(BlueprintPure) AInventory* GetInventoryDefault() { return InventoryClass.GetDefaultObject(); }

	UFUNCTION(BlueprintPure) bool GetIsStorage() { return bIsStorage; }
	void SetIsStorage(bool InIsStorage) { bIsStorage = InIsStorage; }
};
