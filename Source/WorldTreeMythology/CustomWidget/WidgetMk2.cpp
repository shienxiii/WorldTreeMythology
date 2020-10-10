// Copyright of Maple Game Studio


#include "WidgetMk2.h"

bool UWidgetMk2::BindOnWidgetFocused(UObject* InObject, FName InFuncName)
{
	OnWidgetFocused.Unbind();

	OnWidgetFocused.BindUFunction(InObject, InFuncName);

	return OnWidgetFocused.IsBound();
}

void UWidgetMk2::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnWidgetFocused.ExecuteIfBound(this);
}
