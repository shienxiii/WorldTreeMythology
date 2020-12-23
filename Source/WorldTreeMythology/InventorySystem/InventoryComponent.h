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
	// Array of InventoryList managed by this class. Set which InventoryList will go here on InventoryListType 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") TArray<UInventoryList*> Inventory;

	UPROPERTY(EditAnywhere, Category = "Inventory") bool bIsStorage = false;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	void InitializeComponent() override;

	// Call at Construct script to initialize the InventoryList this InventoryComponent will have
	UFUNCTION(BlueprintCallable) void AddInventoryListType(TSubclassOf<UInventoryList> InInventoryList);
	UFUNCTION(BlueprintCallable) void SetIsStorage(bool InIsStorage = false);
	
	/**
	 * Add Inventory to it's respective list. Added Inventory will be in their default base form.
	 * If adding with unique trait, use AddByActor() instead, which will return the InventoryEntry created to add unique traits
	 * 
	 * @return number of items not added to this InventoryComponent
	 */
	UFUNCTION(BlueprintCallable) int32 AddBySubclass(TSubclassOf<AInventory> InInventory, int32 InCount = 1);

	/**
	 * Adds a single unique entry for the passed Inventory class and returns a reference to the entry.
	 * This is used for when picking up Inventory actors from the game world.
	 * 
	 * If the InventoryList is for storing the Inventory class, then automatically call AddBySubclass().
	 *
	 * @return Reference to the InventoryEntry class created
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddByActor(AInventory* InInventory);

	//UFUNCTION(BlueprintCallable) bool AddInventoryByEntry(UInventoryEntry* InInventoryEntry, bool b);


#pragma region InventoryList
	UFUNCTION(BlueprintCallable) TArray<UInventoryEntry*> QueryForSubclass(TSubclassOf<AInventory> InSubclass);
	UFUNCTION(BlueprintCallable) TArray<UInventoryEntry*> CustomQuery(TSubclassOf<AInventory> InInventoryClass, uint8 InQueryEnum);

	/**
	 * Gets the InventoryList that can store the InventoryClass parameter
	 */
	UFUNCTION(BlueprintCallable) UInventoryList* GetInventoryListFor(TSubclassOf<AInventory> InInventoryClass);

	/**
	 * Gets the index of the InventoryList that can store Inventory of class InInventoryClass
	 * 
	 * @param InInventoryClass The Inventory class to test
	 */
	UFUNCTION(BlueprintCallable) int32 IndexOfList(TSubclassOf<AInventory> InInventoryClass);
#pragma endregion
};
