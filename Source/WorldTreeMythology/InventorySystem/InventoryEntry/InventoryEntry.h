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
	TSubclassOf<AInventory> InventoryClass = NULL;
	uint8 Count = 0;

public:
	/**
	 * Initializes the InventoryClass and Count variable if InventoryClass is NULL
	 */
	bool InitializeEntry(TSubclassOf<AInventory> InClass, uint8 InCount);


	void Add(uint8 InCount) { Count += InCount; }

	TSubclassOf<AInventory> GetInventoryClass() { return InventoryClass; }
};
