// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataAssets/Item.h"
#include "PickableActor.generated.h"

UCLASS()
class PATHOGENESIS_API APickableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (ExposeOnSpawn = "true"))
	UItem* ItemData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Count;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPicked;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsEquipped;
};
