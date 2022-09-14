// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Materials/Material.h>
#include "WallAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PATHOGENESIS_API UWallAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterial* WallMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPhysicalMaterial* PhysMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Thickness;
};
