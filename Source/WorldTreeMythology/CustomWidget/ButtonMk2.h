// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMk2.h"
#include "ButtonMk2.generated.h"

class UButton;



/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UButtonMk2 : public UWidgetMk2
{
	GENERATED_BODY()
	
protected:
	// This is the C++ managed Button
	UPROPERTY(meta = (BindWidget)) UButton* CPP_Button;

	// This is the default button style set on the BP side
	FButtonStyle DefaultStyle;

	// This is the style to be swapped to when focused
	FButtonStyle FocusedStyle;

	/**
	 * Used specifically to prevent SynchronizeProperties from running specific code during runtime.
	 * SynchronizeProperties is executed on the editor everytime a UserWidget is compiled or modified, it can be used to preview how this page will look like.
	 * bIsEditor is automatically to be set to false by NativeOnInitialized prevent SynchronizeProperties from running code which consumes resources unecessarily.
	 */
	bool bIsEditor = true;

public:
	UButtonMk2(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;
	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
