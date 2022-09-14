// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UAmmo : public UItem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AmmoDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Penetration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* AmmoMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* AmmoMaterial;
};
