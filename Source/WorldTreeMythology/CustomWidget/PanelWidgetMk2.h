// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "WidgetMk2.h"
#include "PanelWidgetMk2.generated.h"

UENUM(BlueprintType)
enum class EPanelLayout : uint8
{
	VERTICAL UMETA(DisplayName = "Vertical"),
	HORIZONTAL UMETA(DisplayName = "Horizontal"),
	GRID UMETA(DisplayName = "Grid")
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UPanelWidgetMk2 : public UWidgetMk2
{
	GENERATED_BODY()

protected:
	// This is the C++ handled PanelWidget that will manage the UI navigation
	UPROPERTY(meta = (BindWidget)) UPanelWidget* MainPanel;

	// Points to the last user focused child under MainPanel
	UWidget* FocusedChild = nullptr;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup") EPanelLayout PanelLayout = EPanelLayout::VERTICAL;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup", meta = (EditCondition = "PanelLayout == EPanelLayout::GRID")) uint8 GridX = 5;

	FCustomWidgetNavigationDelegate NavigateNext;
	FCustomWidgetNavigationDelegate NavigatePrev;

	FCustomWidgetNavigationDelegate NavigateGrid;
public:
	UPanelWidgetMk2(const FObjectInitializer& ObjectInitializer);
	void NativeOnInitialized() override;

	// Build Navigation for existing child under MainPanel
	UFUNCTION(BlueprintCallable) void RebuildNavigation();
	void BuildNavigation(UWidget* InWidget);

	/**
	 * Spawn and add a child to MainPanel
	 *
	 * @param InChildClass The class of the child to add
	 *
	 * @return The newly added child
	 */
	virtual UWidget* AddChildToPanel(TSubclassOf<UWidget> InChildClass);

	/**
	 * Return the child widget under MainPanel based on the given index
	 *
	 * @param ChildIndex Index of the child to focus on
	 */
	UWidget* GetChildAt(int32 ChildIndex = 0);
	int32 GetChildIndex(UWidget* InWidget);
	int32 GetChildrenCount() { return MainPanel->GetChildrenCount(); }


	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent);
	UFUNCTION() UWidget* GetFocusedChild() { return FocusedChild; }
	UFUNCTION() virtual void SetFocusedChild(UWidget* InWidget) { FocusedChild = InWidget; }
	void ForgetLastFocusedChild() { FocusedChild = nullptr; }


	UFUNCTION() virtual UWidget* NavigateWidget(EUINavigation InNavigation);
	UFUNCTION() virtual UWidget* NavigateGridPanel(EUINavigation InNavigation);

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
