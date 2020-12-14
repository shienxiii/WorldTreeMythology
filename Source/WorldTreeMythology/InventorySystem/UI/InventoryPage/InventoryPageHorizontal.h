// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Slate/SInventoryPageHorizontal.h"
#include "InventoryPage.h"
#include "InventoryPageHorizontal.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UInventoryPageHorizontal : public UInventoryPage
{
	GENERATED_BODY()

protected:
	TSharedPtr<SInventoryPageHorizontal> Panel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Page Setup") uint8 EntryCount = 5;

public:
	int32 GetEntryCount() override { return EntryCount; }

protected:
	void SetupNavigation() override;

	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	void ReleaseSlateResources(bool bReleaseChildren) override;
};
