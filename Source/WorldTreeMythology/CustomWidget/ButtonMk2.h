// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "../WidgetEX/ButtonEX.h"
#include "WidgetMk2.h"
#include "Blueprint/UserWidget.h"
#include "ButtonMk2.generated.h"

/**
 * This is UserWidget class that allows user to design a button appearance and reuse it in another UserWidget object
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class WORLDTREEMYTHOLOGY_API UButtonMk2 : public UWidgetMk2
{
	GENERATED_BODY()
	
protected:
	// This is the C++ managed Button
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) UButtonEX* MainButton;

	UMaterialInstanceDynamic* NormalMatDynamic;
	UMaterialInstanceDynamic* FocusedMatDynamic;
	UMaterialInstanceDynamic* ClickMatDynamic;

public:
	UPROPERTY(BlueprintAssignable) FOnButtonHoverEvent OnHovered;
	UPROPERTY(BlueprintAssignable) FOnButtonClickedEvent OnClicked;

public:
	UButtonMk2(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

	void SetTextureParameter(FName ParamName, UTexture* InTexture);
	void SetIsEnabled(bool InIsEnabled);


	FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION() virtual void NativeHoverEvent();
	UFUNCTION() virtual void NativeClickEvent();

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
