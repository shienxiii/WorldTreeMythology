// Copyright of Maple Game Studio


#include "InventoryEntryDisplay.h"
#include "Components/Button.h"

void UInventoryEntryDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInventoryEntryDisplay::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (bIsEditor)
	{
		// Run editor preview code
	}
}

void UInventoryEntryDisplay::NativeRefresh(TArray<UInventoryEntry*> InQuery)
{
	if (QueryIndex >= InQuery.Num())
	{
		InventoryEntry = nullptr;
		SetTextureParameter(IconParameter, NULL);
		CPP_Button->SetIsEnabled(bEnableOnNull);
	}
	else
	{
		InventoryEntry = InQuery[QueryIndex];
		SetTextureParameter(IconParameter, InventoryEntry->GetInventoryDefault()->GetIcon());
		CPP_Button->SetIsEnabled(true);
	}

	OnRefresh();
}

bool UInventoryEntryDisplay::BindHoverDelegate(UObject* InObject, FName InFuncName)
{
	OnHovered.Unbind();

	OnHovered.BindUFunction(InObject, InFuncName);

	return OnHovered.IsBound();
}

bool UInventoryEntryDisplay::BindClickDelegate(UObject* InObject, FName InFuncName)
{
	OnClicked.Unbind();

	OnClicked.BindUFunction(InObject, InFuncName);

	return OnClicked.IsBound();
}

void UInventoryEntryDisplay::NativeHoverEvent()
{
	Super::NativeHoverEvent();
	OnHovered.ExecuteIfBound(InventoryEntry);
}

void UInventoryEntryDisplay::NativeClickEvent()
{
	Super::NativeClickEvent();
	OnClicked.ExecuteIfBound(InventoryEntry);
}
