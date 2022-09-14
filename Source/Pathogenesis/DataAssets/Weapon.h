// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UWeapon : public UItem
{
	GENERATED_BODY()
	
public:
	UWeapon()
	{
		ItemType = UPathogenesisAssetManager::WeaponItemType;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bIsFullAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMesh* WeaponMesh;

};
