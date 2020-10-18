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

public:
	/**
	 * Initializes the InventoryClass and Count variable if InventoryClass is NULL
	 */
	bool InitializeEntry(TSubclassOf<AInventory> InClass, uint8 InCount);


	void Add(uint8 InCount) { Count += InCount; }

	TSubclassOf<AInventory> GetInventoryClass() { return InventoryClass; }
	AInventory* GetInventoryDefault() { return InventoryClass.GetDefaultObject(); }
	bool IsChildOf(TSubclassOf<AInventory> InBaseClass);
};
