// Copyright of Maple Game Studio

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
/**
 * 
 */
class SInventoryPageHorizontal : public SCompoundWidget
{
protected:
	TSharedPtr<SHorizontalBox> Panel;

public:
	SLATE_BEGIN_ARGS(SInventoryPageHorizontal) {}

	SLATE_END_ARGS()

	/**
		* Construct this widget
		*
		* @param	InArgs	The declaration data for this widget
		*/
	void Construct(const FArguments& InArgs);

	TSharedPtr<SHorizontalBox> GetEntries() { return Panel; }
};