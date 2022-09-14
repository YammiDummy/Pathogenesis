// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "DataAssets/Item.h"
#include "EquipInventoryComponent.generated.h"



UCLASS()
class  UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:

	UEquipInventoryComponent();

public:

	int32 GetMaxItemAmount(int32 SlotIndex, UItem* Item) override;


	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemEquipped, FInventorySlotInfo);
	FOnItemEquipped OnItemEquipped;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUnequipped, FInventorySlotInfo);
	FOnItemUnequipped OnItemUnequipped;

protected:

	UPROPERTY(EditAnywhere)
	TMap<int32, EEquipSlot> EquipSlots;

};
