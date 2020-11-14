// Copyright of Maple Game Studio

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SPanelMk2 : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPanelMk2) {}

	//SLATE_ARGUMENT(TWeakObjectPtr<class APlayerController>, OwningPlayer)
	SLATE_ARGUMENT(TSharedPtr<class APlayerController>, OwningPlayer)
	SLATE_ARGUMENT(TSharedPtr<SWidget>, Button)

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class APlayerController> OwningPlayer;
	TSharedPtr<SWidget> Button;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
