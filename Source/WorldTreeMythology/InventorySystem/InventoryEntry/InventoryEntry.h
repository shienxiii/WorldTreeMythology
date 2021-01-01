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
	UPROPERTY(BlueprintReadOnly) TSubclassOf<AInventoryObject> InventoryClass = NULL;
	UPROPERTY(BlueprintReadOnly) int32 Count = 0;

	// Whether to call ClearEntry() when Count == 0 on Remove()
	UPROPERTY(BlueprintReadOnly, EditAnywhere) bool bClearOnEmpty = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere) bool bIsStorage = false;

public:
	/**
	 * Initializes the InventoryClass and Count variable if InventoryClass is NULL
	 */
	UFUNCTION(BlueprintNativeEvent) bool InitializeEntry(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount = 1);
	bool InitializeEntry_Implementation(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount);
	

	/**
	 * Attempt to increment Count by InCount
	 * 
	 * @param InCount The increment to Count
	 * 
	 * @return Remainder of InCount not added to Count
	 */
	UFUNCTION(BlueprintCallable) int32 Add(int32 InCount = 1);

	/**
	 * Decrement Count by InCount
	 * If Count reaches 0 and bClearOnEmpty is true, nullify this entry
	 * 
	 * @param InCount Number of iten to remove
	 * 
	 * @return Remaining value of Count
	 */
	UFUNCTION(BlueprintCallable) int32 Remove(int32 InCount = 1);

	/**
	 * Clears this instance of InventoryEntry, making it available to be initialized.
	 * Can be overriden in BP
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) void ClearEntry();
	virtual void ClearEntry_Implementation();

	// The InventoryObject held by this class
	UFUNCTION(BlueprintPure) TSubclassOf<AInventoryObject> GetInventoryClass() { return InventoryClass; }

	// The number of InventoryObject held by this InventoryEntry
	UFUNCTION(BlueprintPure) int32 GetCount() { return Count; }

	// Gets the remaining increment that can be added to this InventoryEntry
	UFUNCTION(BlueprintPure) int32 GetRemainingCount();

	// Gets the maximum count of this InventoryEntry
	UFUNCTION(BlueprintPure) int32 GetMaxCount();

	/** 
	 * Check if the InventoryClass stored by this entry is a child of a base class
	 * 
	 * @param InBaseClass The base class to test
	 */
	UFUNCTION(BlueprintPure) bool IsChildOf(TSubclassOf<AInventoryObject> InBaseClass);

	/**
	 * Gets the Class Default Object(CDO) of the InventoryObject held by this InventoryEntry
	 * WARNING: This is for read-only purpose. Modifying the CDO may affect current and future instances of the InventoryObject.
	 * 
	 * @return CDO of the InventoryObject held by this InventoryEntry
	 */
	UFUNCTION(BlueprintPure) AInventoryObject* GetInventoryDefault() { return InventoryClass.GetDefaultObject(); }

	UFUNCTION(BlueprintPure) bool IsEmptyEntry() { return InventoryClass == NULL; }

	UFUNCTION(BlueprintPure) bool IsStorage() { return bIsStorage; }
	void SetIsStorage(bool InIsStorage) { bIsStorage = InIsStorage; }
};
