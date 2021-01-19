// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TestDataTable.generated.h"

USTRUCT(Blueprintable)
struct FTestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 TestNum = 0;
};

/**
 * 
 */
UCLASS(Blueprintable)
class WORLDTREEMYTHOLOGY_API UTestDataTable : public UDataTable
{
	GENERATED_BODY()
	
public:
	UTestDataTable(const FObjectInitializer& ObjectInitializer);
};
