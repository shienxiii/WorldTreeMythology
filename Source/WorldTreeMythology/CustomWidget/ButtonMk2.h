// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMk2.h"
#include "ButtonMk2.generated.h"



/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UButtonMk2 : public UWidgetMk2
{
	GENERATED_BODY()
	
protected:
	// This is the C++ managed Button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UButton* MainButton;

	// This is the default button style set on the BP side
	FButtonStyle DefaultStyle;

	// This is the style to be swapped to when focused
	FButtonStyle FocusedStyle;

	// Pointer to the current style assigned to MainButton
	FButtonStyle* CurrentStyle;

	UMaterialInstanceDynamic* NormalMatDynamic;
	UMaterialInstanceDynamic* FocusedMatDynamic;
	UMaterialInstanceDynamic* ClickMatDynamic;

public:
	UPROPERTY(BlueprintAssignable) FOnButtonHoverEvent OnHovered;
	UPROPERTY(BlueprintAssignable) FOnButtonClickedEvent OnClicked;

public:
	UButtonMk2(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	void SetTextureParameter(FName ParamName, UTexture* InTexture);

	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION() virtual void NativeHoverEvent();
	UFUNCTION() virtual void NativeClickEvent();

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
