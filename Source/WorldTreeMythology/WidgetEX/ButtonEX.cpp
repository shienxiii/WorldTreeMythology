// Copyright of Maple Game Studio


#include "ButtonEX.h"
#include "Components/ButtonSlot.h"
#include "Slate/SButtonEX.h"

UButtonEX::UButtonEX(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Disable this if you do not want hover to set user focus on this button
	OnHovered.AddDynamic(this, &UButtonEX::SetFocus);
}

TSharedRef<SWidget> UButtonEX::RebuildWidget()
{
	MyButton = SNew(SButtonEX)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}