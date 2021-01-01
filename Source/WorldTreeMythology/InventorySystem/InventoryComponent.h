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
	// List of InventoryList this InventoryComponent will contain
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory") TArray<TSubclassOf<UInventoryList>> InventoryLists;

	// Array of InventoryList managed by this class. Set which InventoryList will go here on InventoryListType 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory") TArray<UInventoryList*> Inventory;

	// When set to true, this InventoryComponent will mark all the created InventoryList as a storage
	UPROPERTY(EditAnywhere, Category = "Inventory") bool bIsStorage = false;
public:	
	// Sets default values for this component's properties
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);
	void InitializeComponent() override;

	UFUNCTION(BlueprintCallable) void AddInventoryListType(TSubclassOf<UInventoryList> InInventoryList);

	UFUNCTION(BlueprintCallable) void SetIsStorage(bool InIsStorage = false);
	
	/**
	 * Add Inventory to it's respective list. Added Inventory will be in their default base form.
	 * If adding with unique trait, use AddByActor() instead, which will return the InventoryEntry created to add unique traits
	 * 
	 * @param InInventory Class refence of the InventoryObject to be added
	 * @param InCount 
	 * 
	 * @return number of items not added to this InventoryComponent
	 */
	UFUNCTION(BlueprintCallable) int32 AddBySubclass(TSubclassOf<AInventoryObject> InInventoryClass, int32 InCount = 1);

	/**
	 * Takes an InventoryObject actor and adds it an InventoryList
	 * 
	 * @param InInventory Reference to the InventoryObject actor
	 * @param bDestroyActor Whether to destroy the actor if add is successful
	 *
	 * @return Reference to the InventoryEntry class created
	 */
	UFUNCTION(BlueprintCallable) UInventoryEntry* AddByActor(AInventoryObject* InInventoryClass, bool bDestroyActor = true);


#pragma region InventoryList
	/**
	 * Queries this InventoryComponent for all the InventoryEntry that holds a subclass of an InventoryObject.
	 * 
	 * @param InInventoryClass The base class of the InventoryObject.
	 * @param bEmptyEntries Whether to return a InventoryEntry that holds nothing.
	 * 
	 * @return List of InventoryEntry based on the parameter set.
	 */
	UFUNCTION(BlueprintCallable) TArray<UInventoryEntry*> QueryByBaseClass(TSubclassOf<AInventoryObject> InInventoryClass, bool bClearEmptyEntries = false);


	UFUNCTION(BlueprintCallable) TArray<UInventoryEntry*> QueryBySubclass(TSubclassOf<AInventoryObject> InInventoryClass);
	UFUNCTION(BlueprintCallable) TArray<UInventoryEntry*> CustomQuery(TSubclassOf<AInventoryObject> InInventoryClass, uint8 InQueryEnum);

	/**
	 * Query this InventoryComponent for the number of InventoryObject held.
	 * 
	 * @param InInventoryClass InventoryObject to look for. Must be the exact InventoryObject.
	 * 
	 * @return Number of queried InventoryObject held. returns INDEX_NONE if this InventoryComponent cannot hold the queried InventoryObject.
	 */
	UFUNCTION(BlueprintPure) int32 CountInventory(TSubclassOf<AInventoryObject> InInventoryClass);

	/**
	 * Gets the first InventoryList that can store the InventoryClass parameter.
	 * 
	 * @param InInventoryClass InventoryObject to look for.
	 * 
	 * @return The list that can hold the InventoryObject.
	 */
	UFUNCTION(BlueprintCallable) UInventoryList* GetInventoryListFor(TSubclassOf<AInventoryObject> InInventoryClass);

	/**
	 * Gets all the InventoryList that can store the InventoryClass parameter.
	 *
	 * @param InInventoryClass InventoryObject to look for.
	 *
	 * @return All the list that can hold the InventoryObject.
	 */
	UFUNCTION(BlueprintCallable) TArray<UInventoryList*> GetInventoryListsFor(TSubclassOf<AInventoryObject> InInventoryClass);

#pragma endregion
};
