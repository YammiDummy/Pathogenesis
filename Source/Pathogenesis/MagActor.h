// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "BulletActor.h"
#include "DataAssets/Ammo.h"
#include "MagActor.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API AMagActor : public APickableActor
{
	GENERATED_BODY()
	
public:

	AMagActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UAmmo*> Bullets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MagMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxCapacity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;

};
