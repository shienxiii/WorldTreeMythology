// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryEntry/InventoryList.h"
#include "InventoryComponent.generated.h"

/**
 * The most basic version of the InventoryComponent class.
 * Provides the basic features such as actual list of Inventory entries, adding and removing inventory, query and sort functionalities,
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Determines all the InventoryList that this InventoryComponent will have
	UPROPERTY(EditAnywhere, Category = "Initialization") TArray<TSubclassOf<UInventoryList>> InventoryListType;

	// Array of InventoryList managed by this class. Set which InventoryList will go here on InventoryListType 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") TArray<UInventoryList*> Inventory;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	void InitializeComponent() override;
	

#pragma region InventoryList
	UFUNCTION(BlueprintPure) TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass);
	UFUNCTION(BlueprintPure) TArray<UInventoryEntry*> CustomQuery(TSubclassOf<AInventory> InInventoryClass, uint8 InQueryType);

	/**
	 * Gets the index of the InventoryList that can store Inventory of class InInventoryClass
	 * 
	 * @param InInventoryClass The Inventory class to test
	 */
	UFUNCTION(BlueprintPure) int32 IndexOfList(TSubclassOf<AInventory> InInventoryClass);
#pragma endregion
};
