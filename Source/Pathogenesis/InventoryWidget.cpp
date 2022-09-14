#include "InventoryWidget.h"
#include "DataAssets/Item.h"

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	for (UInventoryCellWidget* CellWidget : CellWidgets)
	{
		InitCellWidget(CellWidget);
		int32 Index = CellWidgets.IndexOfByKey(CellWidget);
		CellWidget->IndexInInventory = Index;
	}
}

void UInventoryWidget::Init(int32 ItemsNum)																					//Initializing main inventory widget. Creating Cell Widgets.
{
	if (ItemCellsGrid)
	{
		ItemCellsGrid->ClearChildren();
		for (int32 i = 0; i < ItemsNum; i++)
		{
			UInventoryCellWidget* Widget = CreateCellWidget();
			if (Widget)
			{
				Widget->IndexInInventory = i;
				ItemCellsGrid->AddChildToUniformGrid(Widget, i / ItemsInRow, i % ItemsInRow);
			}
		}
	}
}

UInventoryCellWidget* UInventoryWidget::CreateCellWidget()
{
	if (CellWidgetClass)
	{
		UInventoryCellWidget* CellWidget = CreateWidget<UInventoryCellWidget>(this, CellWidgetClass);
		CellWidgets.Add(CellWidget);
		//CellWidget->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);
		InitCellWidget(CellWidget);

		return CellWidget;
	}
	return nullptr;
}

void UInventoryWidget::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DraggetTo)
{
	OnItemDrop.Broadcast(DraggedFrom, DraggetTo);
}

void UInventoryWidget::InitCellWidget(UInventoryCellWidget* Cell)
{
	if (Cell)
	{
		Cell->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);
		Cell->ParentInventoryWidget = this;
	}
}

bool UInventoryWidget::AddItem(const FInventorySlotInfo& Item, UItem* ItemInfo, int32 SlotPosition)								//Add item to to inventory slot widget
{
	if (ItemCellsGrid || EquipmentCanvas)
	{
		UInventoryCellWidget* WidgetToAddItem = nullptr;
		UInventoryCellWidget** WidgetToAddItemPtr = CellWidgets.FindByPredicate([SlotPosition](UInventoryCellWidget* Widget)
			{
				return Widget && Widget->IndexInInventory == SlotPosition;
			});

		if (WidgetToAddItemPtr)
		{
			WidgetToAddItem = *WidgetToAddItemPtr;
		}
		else
		{
			for (UInventoryCellWidget* CellWidget : CellWidgets)
			{
				if (!CellWidget->HasItem())
				{
					WidgetToAddItem = CellWidget;
					break;
				}
			}
		}

		if (WidgetToAddItem)
		{
			return WidgetToAddItem->AddItem(Item, *ItemInfo);             //Set data in cell widget
		}
	}
	return false;
}
