// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../InventoryEntry/InventoryEntry.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

DECLARE_DELEGATE_OneParam(FInventoryWidgetFocusEvent, UInventoryWidget*)
DECLARE_DELEGATE_OneParam(FInventoryEntryEvent, UInventoryEntry*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryEntryBPEvent, UInventoryEntry*, InEntry);

/**
 * This class serves as the base for the InventoryButton and InventoryPage class.
 * These classes contains the necessary delegates and callback functions to communicate the player's inventory selection on the UI.
 */
UCLASS(Abstract, NotBlueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// This is the index used by an InventoryWidget to retrieve the InventoryEntry necessary InventoryEntry passed to it
	UPROPERTY(BlueprintReadOnly) int32 Index = 0;

	FInventoryWidgetFocusEvent NativeOnAddedToFocus;

	FInventoryEntryEvent NativeOnEntryHovered;
	FInventoryEntryEvent NativeOnEntryClicked;

public:
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryHovered;
	UPROPERTY(BlueprintAssignable) FInventoryEntryBPEvent OnEntryClicked;


public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void SetIndex(int32 InIndex) { Index = InIndex; }

	bool BindOnAddedToFocus(UObject* InObject, FName InFuncName);
	bool BindNativeOnEntryHovered(UObject* InObject, FName InFuncName);
	bool BindNativeOnEntryClicked(UObject* InObject, FName InFuncName);

	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
};
