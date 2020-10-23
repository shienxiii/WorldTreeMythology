// Copyright of Maple Game Studio


#include "ButtonMk2.h"


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

    CurrentStyle = &DefaultStyle;

    NormalMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(DefaultStyle.Normal.GetResourceObject()), nullptr);
    FocusedMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(FocusedStyle.Normal.GetResourceObject()), nullptr);
    ClickMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(DefaultStyle.Pressed.GetResourceObject()), nullptr);
    
    if (CPP_Button)
    {
        CPP_Button->OnHovered.AddDynamic(CPP_Button, &UButton::SetFocus);

        CPP_Button->OnHovered.AddDynamic(this, &UButtonMk2::NativeHoverEvent);
        CPP_Button->OnClicked.AddDynamic(this, &UButtonMk2::NativeClickEvent);
    }

    bIsEditor = false;
}

void UButtonMk2::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnAddedToFocusPath(InFocusEvent);

    CPP_Button->SetStyle(FocusedStyle);
    CurrentStyle = &FocusedStyle;
}

void UButtonMk2::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnRemovedFromFocusPath(InFocusEvent);

    CPP_Button->SetStyle(DefaultStyle);
    CurrentStyle = &DefaultStyle;
}

void UButtonMk2::SetTextureParameter(FName ParamName, UTexture* InTexture)
{
    NormalMatDynamic->SetTextureParameterValue(ParamName, InTexture);
    FocusedMatDynamic->SetTextureParameterValue(ParamName, InTexture);
    ClickMatDynamic->SetTextureParameterValue(ParamName, InTexture);

    DefaultStyle.Normal.SetResourceObject(NormalMatDynamic);
    DefaultStyle.Hovered.SetResourceObject(NormalMatDynamic);
    DefaultStyle.Pressed.SetResourceObject(ClickMatDynamic);
    
    FocusedStyle.Normal.SetResourceObject(FocusedMatDynamic);
    FocusedStyle.Hovered.SetResourceObject(FocusedMatDynamic);
    FocusedStyle.Pressed.SetResourceObject(ClickMatDynamic);

    CPP_Button->SetStyle(*CurrentStyle);
}

FReply UButtonMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    CPP_Button->SetFocus();
    CPP_Button->OnHovered.Broadcast();
    return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UButtonMk2::NativeHoverEvent()
{
    OnHovered.Broadcast();
}

void UButtonMk2::NativeClickEvent()
{
    OnClicked.Broadcast();
}
