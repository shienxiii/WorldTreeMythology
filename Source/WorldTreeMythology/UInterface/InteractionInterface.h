// Copyright of Maple Game Studio

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

DECLARE_DELEGATE_OneParam(FInteractionEvent, AActor*)

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere) FString InteractionName;
	FInteractionEvent OnInteract;

public:
	FInteractionInfo()
	{
		InteractionName = "Interact";
	}

	FInteractionInfo(FString InInteractionName, UObject* InObj, FName InFuncName)
	{
		InteractionName = InInteractionName;
		OnInteract.BindUFunction(InObj, InFuncName);
	}

	void SetInteractionName(FString InInteractionName)
	{
		InteractionName = InInteractionName;
	}

	bool RebindOnInteractEvent(UObject* InObj, FName InFuncName)
	{
		OnInteract.Unbind();
		OnInteract.BindUFunction(InObj, InFuncName);
		return OnInteract.IsBound();
	}

};

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()


};

/**
 * 
 */
class WORLDTREEMYTHOLOGY_API IInteractionInterface
{
	GENERATED_BODY()

protected:

public:
	virtual FString GetInteractionName() { return "Interact"; }
	virtual void Interact() { return; }
};
