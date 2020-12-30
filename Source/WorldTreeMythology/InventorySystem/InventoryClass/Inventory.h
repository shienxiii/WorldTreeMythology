// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

class UInventoryEntry;

/**
 * This class serves as the base class for all Inventory class.
 * It contains the basic protected access properties and public access functions for identification of an Inventory such as:
 * String - Name, Description, GetName(), GetDescription()
 * Texture - Icon, GetIcon()
 * StaticMeshComponent - Mesh, GetMesh()
 */
UCLASS()
class WORLDTREEMYTHOLOGY_API AInventoryObject : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Name = "Some name";
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Description;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") UTexture* Icon;


	// This is the maximum count of this inventory class can be carried on the player inventory. Only applicable if bUniqueEntries == false in the holding InventoryList
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000")) int32 maxCarry = 10;

	// This is the maximum count of this inventory class can be stored in the player storage. Only applicable if bUniqueEntries == false in the holding InventoryList
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000")) int32 maxStorage = 1000;
	
public:	
	// Sets default values for this actor's properties
	AInventoryObject();

	UFUNCTION(BlueprintPure, BlueprintNativeEvent) FString GetInventoryName();
	virtual FString GetInventoryName_Implementation() { return Name; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent) FString GetDescription();
	virtual FString GetDescription_Implementation() { return Description; }

	UTexture* GetIcon() { return Icon; }

	int32 GetMaxCarry() { return maxCarry; }
	int32 GetMaxStorage() { return maxStorage; }
};
