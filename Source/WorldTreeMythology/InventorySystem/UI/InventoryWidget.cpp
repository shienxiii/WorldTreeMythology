// Copyright of Maple Game Studio


#include "InventoryWidget.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    bIsFocusable = true;
}

bool UInventoryWidget::BindOnAddedToFocus(UObject* InObject, FName InFuncName)
{
    NativeOnAddedToFocus.Unbind();
    NativeOnAddedToFocus.BindUFunction(InObject, InFuncName);
    return NativeOnAddedToFocus.IsBound();
}

bool UInventoryWidget::BindNativeOnEntryHovered(UObject* InObject, FName InFuncName)
{
    NativeOnEntryHovered.Unbind();
    NativeOnEntryHovered.BindUFunction(InObject, InFuncName);
    return NativeOnEntryHovered.IsBound();
}

bool UInventoryWidget::BindNativeOnEntryClicked(UObject* InObject, FName InFuncName)
{
    NativeOnEntryClicked.Unbind();
    NativeOnEntryClicked.BindUFunction(InObject, InFuncName);
    return NativeOnEntryClicked.IsBound();
}

void UInventoryWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnAddedToFocusPath(InFocusEvent);
    NativeOnAddedToFocus.ExecuteIfBound(this);
}
