// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "../../WidgetEX/ButtonEX.h"
#include "../InventoryEntry/InventoryEntry.h"
#include "InventoryButton.generated.h"

UENUM(BlueprintType)
enum class EEntryNullState : uint8
{
	HIDE UMETA(DisplayName = "Hide"),
	DISABLE UMETA(DisplayName = "Disable")
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryButton : public UInventoryWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UButtonEX* EntryButton;

	// If the material used for EntryButton's normal, hover and click contains a texture parameter of this name,
	// sets the texture to the Icon field of the Inventory object pointed by this button
	UPROPERTY(BlueprintReadOnly, EditAnywhere) FName IconParameter = "Icon";

	// Whether to hide or disable this widget when there are no InventoryEntry pointed by it
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EEntryNullState NullState = EEntryNullState::HIDE;

	UInventoryEntry* Entry;
	
	UMaterialInstanceDynamic* NormalMatDynamic;
	UMaterialInstanceDynamic* HoverMatDynamic;
	UMaterialInstanceDynamic* ClickMatDynamic;

public:
	void NativeOnInitialized() override;

	void RefreshQuery(TArray<UInventoryEntry*> InEntry) override;

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	// Checks if there are any InventoryEntry pointed by this widget.
	bool IsNullEntry() { return Entry == nullptr; }

	UFUNCTION() void EntryHoverEvent();
	UFUNCTION() void EntryClickEvent();
};
