// Copyright of Maple Game Studio


#include "ButtonMk2.h"


UButtonMk2::UButtonMk2(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

void UButtonMk2::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    NormalMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(MainButton->WidgetStyle.Normal.GetResourceObject()), nullptr);
    FocusedMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(MainButton->WidgetStyle.Hovered.GetResourceObject()), nullptr);
    ClickMatDynamic = UMaterialInstanceDynamic::Create(Cast<UMaterialInstance>(MainButton->WidgetStyle.Pressed.GetResourceObject()), nullptr);
    
    if (MainButton)
    {
        MainButton->OnHovered.AddDynamic(this, &UButtonMk2::NativeHoverEvent);
        MainButton->OnClicked.AddDynamic(this, &UButtonMk2::NativeClickEvent);
    }
}

void UButtonMk2::SetTextureParameter(FName ParamName, UTexture* InTexture)
{
    NormalMatDynamic->SetTextureParameterValue(ParamName, InTexture);
    FocusedMatDynamic->SetTextureParameterValue(ParamName, InTexture);
    ClickMatDynamic->SetTextureParameterValue(ParamName, InTexture);

    MainButton->WidgetStyle.Normal.SetResourceObject(NormalMatDynamic);
    MainButton->WidgetStyle.Hovered.SetResourceObject(FocusedMatDynamic);
    MainButton->WidgetStyle.Pressed.SetResourceObject(ClickMatDynamic);
}

void UButtonMk2::SetIsEnabled(bool InIsEnabled)
{
    bIsEnabled = InIsEnabled;

    MainButton->SetIsEnabled(InIsEnabled);
}

FReply UButtonMk2::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
    MainButton->SetFocus();
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
