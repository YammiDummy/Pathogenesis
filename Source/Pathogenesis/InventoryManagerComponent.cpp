// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryCellWidget.h"
#include "InventoryComponent.h"
#include "DataAssets/Item.h"
#include "PickableActor.h"
#include "EquipInventoryComponent.h"
#include "WeaponActor.h"
#include "MagActor.h"
#include "AmmoActor.h"
#include "PathogenesisCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryManagerComponent::InitLocalInventory(UInventoryComponent* InInventoryComponent)									//Creating main widget and binding delegates.
{
	if (InInventoryComponent && InventoryWidgetClass)
	{
		if (InventoryWidget)
		{
			InventoryWidget->RemoveFromViewport();
		}

		LocalInventoryComponent = InInventoryComponent;

		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->RepresentedInventory = LocalInventoryComponent;
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));

		InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);

		for (auto& item : LocalInventoryComponent->GetItems())
		{
			UItem* ItemInfo = item.Value.ItemData;
			if (ItemInfo)
			{
				ItemInfo->ItemIcon.LoadSynchronous();
				InventoryWidget->AddItem(item.Value, ItemInfo, item.Key);
			}
		}
	}
}

void UInventoryManagerComponent::ReinitInventoryWidgets(UInventoryComponent* InLocalComp, UInventoryComponent* InEquipComp)
{
	if (InLocalComp)
	{
		InitLocalInventory(InLocalComp);
	}

	if (InEquipComp)
	{
		InitEquipment(InEquipComp);
	}
}

UUserWidget* UInventoryManagerComponent::GetInventoryWidget()
{
	return InventoryWidget;
}

UUserWidget* UInventoryManagerComponent::GetEquipmentWidget()
{
	return EquipInventoryWidget;
}

void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{

	if (DraggedFrom == nullptr || DroppedTo == nullptr)
	{
		return;
	}

	UInventoryComponent* FromInventory = DraggedFrom->ParentInventoryWidget->RepresentedInventory;
	UInventoryComponent* ToInventory = DroppedTo->ParentInventoryWidget->RepresentedInventory;

	if (FromInventory == nullptr || ToInventory == nullptr)
	{
		return;
	}

	FInventorySlotInfo FromItem = DraggedFrom->GetItem();
	FInventorySlotInfo ToItem = DroppedTo->GetItem();

	if (FromItem.ItemActor->Count <= 0)
	{
		return;
	}

	UItem* FromItemData = FromItem.ItemData;
	UItem* ToItemData = ToItem.ItemData;

	if (FromItemData == nullptr)
	{
		return;
	}

	AAmmoActor* AmmoActor = Cast<AAmmoActor>(FromItem.ItemActor);													//Adding ammo into the mag through inventory
	AMagActor* MagActor = Cast<AMagActor>(ToItem.ItemActor);
	if (MagActor && AmmoActor && MagActor->AmmoType == AmmoActor->AmmoType)
	{
		int32 AmmoNeed = MagActor->MaxCapacity - MagActor->Bullets.Num();

		for (int32 Count = 0; Count < AmmoNeed; Count++)
		{
			MagActor->Bullets.Add(Cast<UAmmo>(AmmoActor->ItemData));
			AmmoActor->Count -= 1;

			if (AmmoActor->Count == 0)
			{
				FromInventory->ClearItem(DraggedFrom->IndexInInventory);
				AmmoActor->Destroy();
				break;
			}
		}

		ReinitInventoryWidgets(LocalInventoryComponent, LocalEquipInventoryComponent);
		return;
	}

	MagActor = Cast<AMagActor>(FromItem.ItemActor);																	//Adding mag into the weapon through inventory. Updating delegates.
	AWeaponActor* WeaponActor = Cast<AWeaponActor>(ToItem.ItemActor);
	if (WeaponActor && MagActor && WeaponActor->AmmoType == MagActor->AmmoType)
	{
		if (!WeaponActor->MagActor)
		{
			WeaponActor->MagActor = MagActor;

			FromInventory->ClearItem(DraggedFrom->IndexInInventory);

			UEquipInventoryComponent* EquipInv = Cast<UEquipInventoryComponent>(FromInventory);
			if (EquipInv)
			{
				EquipInv->OnItemUnequipped.Broadcast(FromItem);
			}

			ReinitInventoryWidgets(LocalInventoryComponent, LocalEquipInventoryComponent);
			
			if (WeaponActor->OnMagLoaded.IsBound())
			{
				WeaponActor->OnMagLoaded.Broadcast(MagActor);
			}
			return;
		}
	}

	int32 MaxCount = ToInventory->GetMaxItemAmount(DroppedTo->IndexInInventory, FromItemData);										//Check are able to equip or add.
	if (MaxCount == 0)
	{

		return;
	}

	DraggedFrom->ClearWidget();
																																	//Swapping widget if drag event was successful
	if (ToItemData)
	{
		DraggedFrom->AddItem(ToItem, *ToItemData);
	}

	DroppedTo->ClearWidget();
	DroppedTo->AddItem(FromItem, *FromItemData);


	FromInventory->SetItem(DraggedFrom->IndexInInventory, ToItem);
	ToInventory->SetItem(DroppedTo->IndexInInventory, FromItem);

	UEquipInventoryComponent* EquipInv = Cast<UEquipInventoryComponent>(FromInventory);												//Additional checks. Watch PathogenesisCharacter::EquipItem/UnequipItem.
	if (EquipInv)
	{
		EquipInv->OnItemUnequipped.Broadcast(FromItem);
	}

	EquipInv = Cast<UEquipInventoryComponent>(ToInventory);
	if (EquipInv)
	{
		EquipInv->OnItemEquipped.Broadcast(FromItem);
	}

}

void UInventoryManagerComponent::InitEquipment(UInventoryComponent* InInventoryComponent)
{
	LocalEquipInventoryComponent = InInventoryComponent;

	if (EquipInventoryWidget)
	{
		EquipInventoryWidget->RemoveFromViewport();
	}

	if (EquipInventoryWidgetClass)
	{
		EquipInventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), EquipInventoryWidgetClass);
		EquipInventoryWidget->RepresentedInventory = InInventoryComponent;

		EquipInventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
		EquipInventoryWidget->AddToViewport();

		for (auto& item : LocalEquipInventoryComponent->GetItems())
		{
			UItem* ItemInfo = item.Value.ItemData;
			if (ItemInfo)
			{
				ItemInfo->ItemIcon.LoadSynchronous();
				EquipInventoryWidget->AddItem(item.Value, ItemInfo, item.Key);
			}
		}
	}
}

