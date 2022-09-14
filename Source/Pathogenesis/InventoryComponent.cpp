// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "DataAssets/Item.h"
#include "PickableActor.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FInventorySlotInfo* UInventoryComponent::GetItem(int32 SlotIndex)
{
	return Items.Find(SlotIndex);
}

void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo Item)
{	
	ClearItem(SlotIndex);
		
	if (Item.ItemActor && Item.ItemActor>0)	
	{	
		Items.Add(SlotIndex, Item);
	}
	
}

void UInventoryComponent::ClearItems()
{
	Items.Empty();
}

void UInventoryComponent::ClearItem(int32 SlotIndex)
{

	Items.Remove(SlotIndex);
}

const TMap<int32, FInventorySlotInfo>& UInventoryComponent::GetItems()
{
	return Items;
}

void UInventoryComponent::SetItems(TMap<int32, FInventorySlotInfo> InItems)
{
	Items.Empty();
	Items = InItems;
}

int32 UInventoryComponent::GetIndex(const APickableActor* Actor)
{
	for (auto& item : Items)
	{
		if (item.Value.ItemActor == Actor)
		{
			return item.Key;
		}
	}
	return -1;
}

int32 UInventoryComponent::GetItemsNum()
{
	return Items.Num();
}

int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, UItem* Item)
{
	FInventorySlotInfo* InfoPtr = Items.Find(SlotIndex);
	if (InfoPtr && InfoPtr->ItemData == Item)
	{
		return 0;
	}
	return -1;
}

