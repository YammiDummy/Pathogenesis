// Fill out your copyright notice in the Description page of Project Settings.


#include "PathogenesisAssetManager.h"
#include "DataAssets/Item.h"


const FPrimaryAssetType UPathogenesisAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType UPathogenesisAssetManager::AmmoItemType = TEXT("Ammo");
const FPrimaryAssetType UPathogenesisAssetManager::GearItemType = TEXT("Gear");
const FPrimaryAssetType UPathogenesisAssetManager::SuppliesItemType = TEXT("Supplies");


void UPathogenesisAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

UPathogenesisAssetManager& UPathogenesisAssetManager::Get()
{

	UPathogenesisAssetManager* This = Cast<UPathogenesisAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		return *NewObject<UPathogenesisAssetManager>();
	}
}

void UPathogenesisAssetManager::AssetManagerLoadItems()
{
	UPathogenesisAssetManager& AssetManager = UPathogenesisAssetManager::Get();

	TArray<FPrimaryAssetId> WeaponIdList;
	AssetManager.GetPrimaryAssetIdList(WeaponItemType, WeaponIdList);


	for (const FPrimaryAssetId& WeaponId : WeaponIdList)
	{
		FAssetData AssetDataToParse;
		AssetManager.GetPrimaryAssetData(WeaponId, AssetDataToParse);

		FName QueryExample;
		AssetDataToParse.GetTagValue(GET_MEMBER_NAME_CHECKED(UItem, RegistryTag), QueryExample);
	}

	TArray<FName> CurrentLoadState;
	CurrentLoadState.Add(FName("Game"));
}

