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
	UWidget* LastFocusedChild = nullptr;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Panel Setup") EPanelLayout NavigationDirection = EPanelLayout::VERTICAL;
	FCustomWidgetNavigationDelegate NavigateNext;
	FCustomWidgetNavigationDelegate NavigatePrev;
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

	UFUNCTION() UWidget* GetLastFocusedChild() { return LastFocusedChild; }
	UFUNCTION() virtual void SetLastFocusedChild(UWidget* InWidget) { LastFocusedChild = InWidget; }
	void ForgetLastFocusedChild() { LastFocusedChild = nullptr; }


	UFUNCTION() virtual UWidget* NavigateWidget(EUINavigation InNavigation);

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
