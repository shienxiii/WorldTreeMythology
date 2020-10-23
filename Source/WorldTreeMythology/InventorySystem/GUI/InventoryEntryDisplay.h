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
	UPROPERTY(BlueprintReadOnly) UInventoryEntry* InventoryEntry = nullptr;
	UPROPERTY(BlueprintReadOnly) int32 QueryIndex = 0;

	/**
	 * Name of the parameter on the material used on CPP_Button to display the Inventory Icon property, if applicable
	 */
	UPROPERTY(EditAnywhere) FName IconParameter = "Icon";

	// Enable this entry display when it does not store anything?
	UPROPERTY(EditAnywhere) bool bEnableOnNull = false;

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

	void NativeHoverEvent() override;
	void NativeClickEvent() override;
#pragma endregion

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("Inventory UI"); }
#endif
};
