// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "BulletActor.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API ABulletActor : public AActor
{
	GENERATED_BODY()

public:

	ABulletActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = "true"))
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, Meta = (ExposeOnSpawn = "true"))
	UStaticMesh* BulletMesh; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* BulletMeshComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = "true"))
	float Penetration;
};
