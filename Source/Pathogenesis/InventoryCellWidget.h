// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataAssets/Item.h"
#include "InventoryCellWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget*, UInventoryCellWidget*);

/**
 *
 */
UCLASS()
class UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	bool HasItem() const { return bHasItem; };

	bool AddItem(const FInventorySlotInfo& Item, const UItem& ItemInfo);

	void ClearWidget();

	const FInventorySlotInfo& GetItem();

	int32 IndexInInventory = -1;

protected:

	bool bHasItem;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* InnerText;

	UPROPERTY()
	FInventorySlotInfo StoredItem;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditDefaultsOnly)
	bool bIsDraggable = true;

public:

	FOnItemDrop OnItemDrop;

	UPROPERTY()
	class UInventoryWidget* ParentInventoryWidget;
};
