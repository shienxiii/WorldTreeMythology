// Copyright of Maple Game Studio


#include "SButtonEX.h"

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SButtonEX::Construct(const FArguments& InArgs)
{
	bIsPressed = false;

	// Text overrides button content. If nothing is specified, put an null widget in the button.
	// Null content makes the button enter a special mode where it will ask to be as big as the image used for its border.
	struct
	{
		TSharedRef<SWidget> operator()(const FArguments& InOpArgs) const
		{
			if ((InOpArgs._Content.Widget == SNullWidget::NullWidget) && (InOpArgs._Text.IsBound() || !InOpArgs._Text.Get().IsEmpty()))
			{
				return SNew(STextBlock)
					.Visibility(EVisibility::HitTestInvisible)
					.Text(InOpArgs._Text)
					.TextStyle(InOpArgs._TextStyle)
					.TextShapingMethod(InOpArgs._TextShapingMethod)
					.TextFlowDirection(InOpArgs._TextFlowDirection);
			}
			else
			{
				return InOpArgs._Content.Widget;
			}
		}
	} DetermineContent;

	SBorder::Construct(SBorder::FArguments()
		.ContentScale(InArgs._ContentScale)
		.DesiredSizeScale(InArgs._DesiredSizeScale)
		.BorderBackgroundColor(InArgs._ButtonColorAndOpacity)
		.ForegroundColor(InArgs._ForegroundColor)
		.BorderImage(this, &SButtonEX::GetBorder)
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		.Padding(TAttribute<FMargin>(this, &SButtonEX::GetCombinedPadding))
		.ShowEffectWhenDisabled(TAttribute<bool>(this, &SButtonEX::GetShowDisabledEffect))
		[
			DetermineContent(InArgs)
		]
	);

	// Only do this if we're exactly an SButton
	/*if (GetType() == SButtonTypeName)
	{
		SetCanTick(false);
	}*/
	SetCanTick(false);

	ContentPadding = InArgs._ContentPadding;

	SetButtonStyle(InArgs._ButtonStyle);

	bIsFocusable = InArgs._IsFocusable;

	OnClicked = InArgs._OnClicked;
	OnPressed = InArgs._OnPressed;
	OnReleased = InArgs._OnReleased;
	OnHovered = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;

	ClickMethod = InArgs._ClickMethod;
	TouchMethod = InArgs._TouchMethod;
	PressMethod = InArgs._PressMethod;

	HoveredSound = InArgs._HoveredSoundOverride.Get(Style->HoveredSlateSound);
	PressedSound = InArgs._PressedSoundOverride.Get(Style->PressedSlateSound);
}

FReply SButtonEX::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	bIsFocused = true;
	return FReply::Handled();
}

void SButtonEX::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	bIsFocused = false;
	SButton::OnFocusLost(InFocusEvent);
}
