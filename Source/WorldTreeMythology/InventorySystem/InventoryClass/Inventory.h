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

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Name;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") FString Description;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory") UTexture* Icon;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	FString GetName() { return Name; }
	FString GetDescription() { return Description; }
	UTexture* GetIcon() { return Icon; }

	UStaticMeshComponent* GetMesh() { return Mesh; }
};
