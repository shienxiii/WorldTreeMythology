// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "InventoryPanel.h"
#include "InventoryPanelScroll.generated.h"

/**
 * This is an InventoryPanel class that stores the InventoryWidgets under a ScrollBox widget.
 * User of this class will have to manually add a ScrollBox named Panel on the Widget Designer in Blueprint.
 * This is to allow the user to have more control on the appearance of the ScrollBox.
 */
UCLASS(Blueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryPanelScroll : public UInventoryPanel
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UScrollBox* Panel;

protected:
	void NativePreConstruct() override;

	void SetupNavigation(UWidget* InWidget) override;

public:
	void RefreshPanel(TArray<UInventoryEntry*> InQuery);
	UFUNCTION(BlueprintCallable) void ResizePanel(int32 InEntryCount);

};
