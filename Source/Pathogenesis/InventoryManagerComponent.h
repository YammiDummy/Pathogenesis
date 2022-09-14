// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventoryManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitLocalInventory(UInventoryComponent* InInventoryComponent);

	void InitEquipment(UInventoryComponent* InInventoryComponent);

	//FInventoryItemInfo* GetItemData(FName ItemId);

	UFUNCTION(BlueprintCallable)
	void ReinitInventoryWidgets(UInventoryComponent* InLocalComp, UInventoryComponent* InEquipComp);

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetInventoryWidget();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetEquipmentWidget();

	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* EquipInventoryWidget;

protected:

	UPROPERTY()
	UInventoryComponent* LocalInventoryComponent;

	UPROPERTY()
	UInventoryComponent* LocalEquipInventoryComponent;

	//UPROPERTY(EditAnywhere)
	//UDataTable* InventoryItemData;



	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	int32 MinInventorySize = 20;

	void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DraggedTo);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> EquipInventoryWidgetClass;
};
