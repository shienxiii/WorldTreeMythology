// Copyright of Maple Game Studio


#include "SInventoryPageHorizontal.h"
#include "Widgets/Layout/SConstraintCanvas.h"

void SInventoryPageHorizontal::Construct(const FArguments& InArgs)
{
    Panel = SNew(SHorizontalBox);

    ChildSlot
        [
            SNew(SConstraintCanvas)
            + SConstraintCanvas::Slot()
            .Alignment(FVector2D(0.0f, 0.0f))
            .AutoSize(true)
            [
                Panel.ToSharedRef()
            ]
        ];
}
