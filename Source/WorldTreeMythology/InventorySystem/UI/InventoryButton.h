// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "InventoryWidget.h"
#include "../../WidgetEX/ButtonEX.h"
#include "../InventoryEntry/InventoryEntry.h"
#include "InventoryButton.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryButton : public UInventoryWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget)) UButtonEX* EntryButton;

	UInventoryEntry* Entry;
	

public:
	void NativePreConstruct() override;

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION() void EntryHoverEvent();
	UFUNCTION() void EntryClickEvent();
};
