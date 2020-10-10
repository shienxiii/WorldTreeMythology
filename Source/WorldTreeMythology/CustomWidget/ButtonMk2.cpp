// Copyright of Maple Game Studio


#include "ButtonMk2.h"
#include "Components/Button.h"

UButtonMk2::UButtonMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UButtonMk2::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    DefaultStyle = CPP_Button->WidgetStyle;
    FocusedStyle = CPP_Button->WidgetStyle;
    DefaultStyle.SetHovered(DefaultStyle.Normal);
    FocusedStyle.SetNormal(FocusedStyle.Hovered);

    CPP_Button->OnHovered.AddDynamic(CPP_Button, &UButton::SetFocus);
    bIsEditor = false;
}

void UButtonMk2::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnAddedToFocusPath(InFocusEvent);
    CPP_Button->SetStyle(FocusedStyle);
}

void UButtonMk2::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnRemovedFromFocusPath(InFocusEvent);
    CPP_Button->SetStyle(DefaultStyle);
}

FReply UButtonMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    CPP_Button->SetFocus();
    CPP_Button->OnHovered.Broadcast();
    return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}
