// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "PathogenesisAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UPathogenesisAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UPathogenesisAssetManager() {}
	virtual void StartInitialLoading() override;

	static const FPrimaryAssetType WeaponItemType;
	static const FPrimaryAssetType AmmoItemType;
	static const FPrimaryAssetType GearItemType;
	static const FPrimaryAssetType SuppliesItemType;

	static UPathogenesisAssetManager& Get();

	void AssetManagerLoadItems();
};
