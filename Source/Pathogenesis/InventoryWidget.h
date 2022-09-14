// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryCellWidget.h"
#include <Components/CanvasPanel.h>
#include "InventoryWidget.generated.h"


class UInventoryCellWidget;
struct FInventorySlotInfo;
class UItem;

/**
 *
 */

UCLASS()
class UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void Init(int32 ItemsNum);

	UFUNCTION(BlueprintCallable)
	bool AddItem(const FInventorySlotInfo& Item, UItem* ItemInfo, int32 SlotPosition = -1);

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* ItemCellsGrid;

	UPROPERTY(meta = (BindWidgetOptional))
	UCanvasPanel* EquipmentCanvas;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;

	UPROPERTY(meta = (BindWidgetOptional))
	UInventoryCellWidget* GoldCell;

	UInventoryCellWidget* CreateCellWidget();

	void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DraggetTo);

public:

	FOnItemDrop OnItemDrop;

private:

	void InitCellWidget(UInventoryCellWidget* Cell);

public:

	UPROPERTY()
	class UInventoryComponent* RepresentedInventory;
};
