// Copyright of Maple Game Studio


#include "TestDataTable.h"

UTestDataTable::UTestDataTable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    RowStruct = FTestData::StaticStruct();
}