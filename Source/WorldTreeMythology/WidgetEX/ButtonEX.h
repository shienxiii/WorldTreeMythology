// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ButtonEX.generated.h"


/**
 * 
 */
UCLASS()
class WORLDTREEMYTHOLOGY_API UButtonEX : public UButton
{
	GENERATED_BODY()

public:
	
protected:
	UButtonEX(const FObjectInitializer& ObjectInitializer);

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
