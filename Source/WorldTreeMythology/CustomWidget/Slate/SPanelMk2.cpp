// Copyright of Maple Game Studio


#include "SPanelMk2.h"

#define LOCTEXT_NAMESPACE "SlateTest"

void SPanelMk2::Construct(const FArguments& InArgs)
{
    Button = InArgs._Button;

    if (!Button.IsValid()) { return; }
    bCanSupportFocus = true;

    //const FMargin ContentPadding = FMargin(500.0f, 300.0f);
    const FText TestText = LOCTEXT("Title", "Slate Test");

    TSharedPtr<SVerticalBox> Container = SNew(SVerticalBox);
    for (int i = 0; i < 3; i++)
    {
        Container->AddSlot()
            [
                SNew(STextBlock)
                .Text(TestText)
            ];
    }

    ChildSlot
        [
            SNew(SOverlay) // Create an overlay
            + SOverlay::Slot() // Add a slot to the overlay
                .HAlign(EHorizontalAlignment::HAlign_Fill)
                .VAlign(EVerticalAlignment::VAlign_Fill)
                [
                    SNew(SImage)
                    .ColorAndOpacity(FColor::Black)
                ]
                + SOverlay::Slot()
                    .HAlign(EHorizontalAlignment::HAlign_Fill)
                    .VAlign(EVerticalAlignment::VAlign_Fill)
                    //.Padding(ContentPadding)
                    [
                    //Container.ToSharedRef()
                    Button.ToSharedRef()
                    /*SNew(SVerticalBox)
                    + SVerticalBox::Slot()
                    [
                        SNew(STextBlock)
                        .Text(TestText)
                    ]*/
                ]
        ];

}

#undef LOCTEXT_NAMESPACE 