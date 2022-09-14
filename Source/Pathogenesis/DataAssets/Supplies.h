// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Supplies.generated.h"

UENUM(BlueprintType)
enum class 	ESuppliesType : uint8
{
	ST_Medkit,
	ST_Drink
};

UCLASS()
class PATHOGENESIS_API USupplies : public UItem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESuppliesType Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Power;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* SupplyMesh;


};
