// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../../CustomWidget/ButtonMk2.h"
#include "../InventoryEntry/InventoryEntry.h"
#include "InventoryEntryDisplay.generated.h"

DECLARE_DELEGATE_OneParam(FInventoryEntryEvent, UInventoryEntry*)

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class WORLDTREEMYTHOLOGY_API UInventoryEntryDisplay : public UButtonMk2
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly) UInventoryEntry* Entry = nullptr;
	UPROPERTY(BlueprintReadOnly) int32 QueryIndex = 0;

	FInventoryEntryEvent OnHovered;
	FInventoryEntryEvent OnClicked;

public:
	void NativeOnInitialized() override;
	void SynchronizeProperties() override;

	void SetQueryIndex(int32 InQueryIndex) { QueryIndex = InQueryIndex; }
	void NativeRefresh(TArray<UInventoryEntry*> InQuery);

	/**
	 * Blueprint event to run when the Entry property gets updated
	 */
	UFUNCTION(BlueprintImplementableEvent) void OnRefresh();

#pragma region Event
	bool BindHoverDelegate(UObject* InObject, FName InFuncName);
	bool BindClickDelegate(UObject* InObject, FName InFuncName);

	UFUNCTION() void NativeHoverEvent();
	UFUNCTION() void NativeClickEvent();
#pragma endregion

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("Inventory UI"); }
#endif
};
