// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Gear.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UGear : public UItem
{
	GENERATED_BODY()
	
public:
	UGear()
	{
		ItemType = UPathogenesisAssetManager::GearItemType;
	}
};
