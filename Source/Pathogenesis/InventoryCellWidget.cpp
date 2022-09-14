// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCellWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "WeaponActor.h"
#include "PickableActor.h"
#include <Kismet/GameplayStatics.h>
#include "PathogenesisCharacter.h"

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& Item, const UItem& ItemInfo)												
{
	if (bHasItem)
	{
		return false;
	}

	if (Item.ItemActor->Count == 0)
	{
		ClearWidget();
		return true;
	}

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemInfo.ItemIcon.Get());
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Item.ItemActor->Count)));
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	AWeaponActor* Weapon = Cast<AWeaponActor>(Item.ItemActor);
	if (Weapon)
	{
		if (Weapon->MagActor)
		{
			InnerText->SetText(FText::FromString(FString::FromInt(Weapon->MagActor->Bullets.Num())));
		}
		else
		{
			InnerText->SetText(FText::FromString(TEXT("No Mag")));
		}
		InnerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	AMagActor* Mag = Cast<AMagActor>(Item.ItemActor);
	if (Mag)
	{
		InnerText->SetText(FText::FromString(FString::FromInt(Mag->Bullets.Num())));
		InnerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	bHasItem = true;
	StoredItem = Item;

	return true;
}

void UInventoryCellWidget::ClearWidget()
{
	if (ItemImage)
	{
		ItemImage->SetBrush(FSlateBrush());
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString("0"));
		CountText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (InnerText)
	{
		InnerText->SetVisibility(ESlateVisibility::Hidden);
	}

	bHasItem = false;
	StoredItem = FInventorySlotInfo();
}

const FInventorySlotInfo& UInventoryCellWidget::GetItem()
{
	return StoredItem;
}

FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{


	if (bIsDraggable && bHasItem && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{

		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	AWeaponActor* Weapon = Cast<AWeaponActor>(GetItem().ItemActor);																			//Disarm weapon if RMB clicked
	if (Weapon && Weapon->MagActor && InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		APathogenesisCharacter* Char = Cast<APathogenesisCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (Char)
		{
			Char->PickUpItem(Weapon->MagActor);
			Weapon->MagActor->bIsEquipped = false;
			Weapon->MagActor = nullptr;

			if (Weapon->OnMagLoaded.IsBound())
			{
				Weapon->OnMagLoaded.Broadcast(Weapon->MagActor);
			}

			Char->ReinitWidgets();
		}
	}

	//Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{


	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Drag"), true));
	
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	
	if (OutOperation)
	{
		OutOperation->DefaultDragVisual = this;
		OutOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::HitTestInvisible);
		OutOperation->Pivot = EDragPivot::MouseDown;
	}
}

bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryCellWidget* OtherCell = InOperation ? Cast<UInventoryCellWidget>(InOperation->DefaultDragVisual) : nullptr;

	Cast<UInventoryCellWidget>(InOperation->DefaultDragVisual)->SetVisibility(ESlateVisibility::Visible);
	InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Visible);
	this->SetVisibility(ESlateVisibility::Visible);

	if (OtherCell && OtherCell != this)
	{
		OnItemDrop.Broadcast(OtherCell, this);
		return true;
	}
	return false;
	//return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventoryCellWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Visible);
}
