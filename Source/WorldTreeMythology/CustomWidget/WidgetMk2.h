// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetMk2.generated.h"


DECLARE_DELEGATE_OneParam(FWidgetFocused, UWidget*)

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class WORLDTREEMYTHOLOGY_API UWidgetMk2 : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	FWidgetFocused OnWidgetFocused;

public:
	bool BindOnWidgetFocused(UObject* InObject, FName InFuncName);
	void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;

#if WITH_EDITOR
	const FText GetPaletteCategory() override { return FText::FromString("UMG Mk2"); }
#endif
};
