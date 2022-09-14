// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipInventoryComponent.h"
#include "DataAssets/Item.h"

UEquipInventoryComponent::UEquipInventoryComponent()
{
	EquipSlots.Add(0, EEquipSlot::ES_Head);
	EquipSlots.Add(1, EEquipSlot::ES_Body);
	EquipSlots.Add(2, EEquipSlot::ES_Primary);
	EquipSlots.Add(3, EEquipSlot::ES_Secondary);
	EquipSlots.Add(4, EEquipSlot::ES_Vest);
	EquipSlots.Add(5, EEquipSlot::ES_Vest);
	EquipSlots.Add(6, EEquipSlot::ES_Vest);
	EquipSlots.Add(7, EEquipSlot::ES_Supply);
}

int32 UEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, UItem* Item)
{
	if (!EquipSlots.Contains(SlotIndex) || EquipSlots.FindChecked(SlotIndex) != Item->EquipSlot)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("test")), true);
		return 0;
	}

	return 1;
}
