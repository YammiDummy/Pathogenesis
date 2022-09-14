// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../PathogenesisAssetManager.h"
#include "Item.generated.h"


UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	ES_None,
	ES_Head,
	ES_Body,
	ES_Primary,
	ES_Secondary,
	ES_Vest,
	ES_Supply
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AT_9mm,
	AT_556mm,
	AT_545mm
};


UCLASS(Abstract, BlueprintType)
class PATHOGENESIS_API UItem : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UItem()
		: MaxCount(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	EEquipSlot EquipSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 MaxCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, AssetRegistrySearchable)
	FName RegistryTag;

};


USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	UItem* ItemData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
	class APickableActor* ItemActor;
};

