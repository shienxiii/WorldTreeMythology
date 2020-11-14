// Copyright of Maple Game Studio


#include "InventoryEntryButton.h"
#include "Components/Button.h"

void UInventoryEntryButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInventoryEntryButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UInventoryEntryButton::NativeRefresh(TArray<UInventoryEntry*> InQuery)
{
	if (QueryIndex >= InQuery.Num())
	{
		InventoryEntry = nullptr;
		SetTextureParameter(IconParameter, NULL);
		SetIsEnabled(bEnableOnNull);
	}
	else
	{
		InventoryEntry = InQuery[QueryIndex];
		if (InventoryEntry->GetInventoryClass())
		{
			SetTextureParameter(IconParameter, InventoryEntry->GetInventoryDefault()->GetIcon());
		}
		SetIsEnabled(true);
	}

	OnRefresh();
}

bool UInventoryEntryButton::BindHoverDelegate(UObject* InObject, FName InFuncName)
{
	OnHovered.Unbind();

	OnHovered.BindUFunction(InObject, InFuncName);

	return OnHovered.IsBound();
}

bool UInventoryEntryButton::BindClickDelegate(UObject* InObject, FName InFuncName)
{
	OnClicked.Unbind();

	OnClicked.BindUFunction(InObject, InFuncName);

	return OnClicked.IsBound();
}

void UInventoryEntryButton::NativeHoverEvent()
{
	Super::NativeHoverEvent();
	OnHovered.ExecuteIfBound(InventoryEntry);
}

void UInventoryEntryButton::NativeClickEvent()
{
	Super::NativeClickEvent();
	OnClicked.ExecuteIfBound(InventoryEntry);
}
