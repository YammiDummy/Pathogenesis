// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "AmmoActor.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API AAmmoActor : public APickableActor
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;	
};
