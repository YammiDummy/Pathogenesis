// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"



class UItem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FInventorySlotInfo* GetItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo Item);

	UFUNCTION(BlueprintCallable)
	void ClearItems();

	void ClearItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const TMap<int32, FInventorySlotInfo>& GetItems();

	UFUNCTION(BlueprintCallable)
	void SetItems(TMap<int32, FInventorySlotInfo> InItems);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetIndex(const APickableActor* Actor);


	int32 GetItemsNum();

protected:

	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

public:

	virtual int32 GetMaxItemAmount(int32 SlotIndex, UItem* Item);

};
