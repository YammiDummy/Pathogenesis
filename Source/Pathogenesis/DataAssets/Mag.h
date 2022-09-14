// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Mag.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UMag : public UItem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MagCapacity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* MagMesh;

};
