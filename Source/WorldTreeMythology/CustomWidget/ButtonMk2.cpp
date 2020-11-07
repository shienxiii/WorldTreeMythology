// Copyright of Maple Game Studio


#include "ButtonMk2.h"


UButtonMk2::UButtonMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UButtonMk2::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    DefaultStyle = MainButton->WidgetStyle;
    FocusedStyle = MainButton->WidgetStyle;
    DisabledStyle = MainButton->WidgetStyle;

    DefaultStyle.SetHovered(DefaultStyle.Normal);
    FocusedStyle.SetNormal(FocusedStyle.Hovered);
    DisabledStyle.SetNormal(DisabledStyle.Disabled);


    CurrentStyle = &DefaultStyle;

    NormalMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(DefaultStyle.Normal.GetResourceObject()), nullptr);
    FocusedMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(FocusedStyle.Normal.GetResourceObject()), nullptr);
    ClickMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(DefaultStyle.Pressed.GetResourceObject()), nullptr);
    
    if (MainButton)
    {
        MainButton->OnHovered.AddDynamic(MainButton, &UButton::SetFocus);

        MainButton->OnHovered.AddDynamic(this, &UButtonMk2::NativeHoverEvent);
        MainButton->OnClicked.AddDynamic(this, &UButtonMk2::NativeClickEvent);
    }
}

void UButtonMk2::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnAddedToFocusPath(InFocusEvent);

    MainButton->SetStyle(FocusedStyle);
    CurrentStyle = &FocusedStyle;
}

void UButtonMk2::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
    Super::NativeOnRemovedFromFocusPath(InFocusEvent);

    MainButton->SetStyle(DefaultStyle);
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

    MainButton->SetStyle(*CurrentStyle);
}

void UButtonMk2::SetIsEnabled(bool InIsEnabled)
{
    bIsEnabled = InIsEnabled;

    if (InIsEnabled) { EnableButton(); }
    else { DisableButton(); }
}

void UButtonMk2::DisableButton()
{
    MainButton->SetIsEnabled(false);
    MainButton->SetStyle(DisabledStyle);
    CurrentStyle = &DisabledStyle;
}

void UButtonMk2::EnableButton()
{
    MainButton->SetIsEnabled(true);
    MainButton->SetStyle(DefaultStyle);
    CurrentStyle = &DefaultStyle;
}

FReply UButtonMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    MainButton->SetFocus();
    MainButton->OnHovered.Broadcast();
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
