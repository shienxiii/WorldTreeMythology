// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

/**
 * This class serves as the base class for all Inventory class.
 * It contains the basic protected access properties and public access functions for identification of an Inventory such as:
 * String - Name, Description, GetName(), GetDescription()
 * Texture - Icon, GetIcon()
 * StaticMeshComponent - Mesh, GetMesh()
 */
UCLASS()
class WORLDTREEMYTHOLOGY_API AInventory : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Name = "Some name";
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Description;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") UTexture* Icon;

	// This is the maximum count of this inventory class can be carried on the player inventory. Only applicable if bUniqueEntries == false in the holding InventoryList
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0")) int32 maxCarry = 10;

	// This is the maximum count of this inventory class can be stored in the player storage. Only applicable if bUniqueEntries == false in the holding InventoryList
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0.0", UIMax = "1000.0")) int32 maxStorage = 1000;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	FString GetInventoryName() { return Name; }
	FString GetDescription() { return Description; }
	UTexture* GetIcon() { return Icon; }

	UStaticMeshComponent* GetMesh() { return Mesh; }
};
