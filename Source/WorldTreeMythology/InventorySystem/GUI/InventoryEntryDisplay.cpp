// Copyright of Maple Game Studio


#include "InventoryEntryDisplay.h"
#include "Components/Button.h"

void UInventoryEntryDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CPP_Button)
	{
		CPP_Button->OnHovered.AddDynamic(this, &UInventoryEntryDisplay::NativeHoverEvent);
		CPP_Button->OnClicked.AddDynamic(this, &UInventoryEntryDisplay::NativeClickEvent);
	}
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
	Entry = InQuery[QueryIndex];

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
	OnHovered.ExecuteIfBound(Entry);
}

void UInventoryEntryDisplay::NativeClickEvent()
{
	OnClicked.ExecuteIfBound(Entry);
}
