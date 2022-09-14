// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathogenesisCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "EquipInventoryComponent.h"
#include "InventoryManagerComponent.h"
#include "WeaponActor.h"
#include "DataAssets/Ammo.h"
#include "DataAssets/Mag.h"
#include "PathogenesisMedKit.h"
#include "HealthComponent.h"

APathogenesisCharacter::APathogenesisCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level


	//FName CurrentWeaponSocket = TEXT("hand_rSocket");
	//CurrentWeapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("CurrentWeapon"));
	//CurrentWeapon->SetupAttachment(GetMesh(), CurrentWeaponSocket);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EquipmentInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>(TEXT("Equipment Inventory Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("Inventory Manager Component"));

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	TeamId = FGenericTeamId(0);



	HealthManager = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Manager"));

}

void APathogenesisCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void APathogenesisCharacter::OpenInventory()
{
	InventoryManagerComponent->InitLocalInventory(InventoryComponent);
	InventoryManagerComponent->InitEquipment(EquipmentInventoryComponent);
}

void APathogenesisCharacter::CloseInventory()
{
	InventoryManagerComponent->InventoryWidget->RemoveFromViewport();
	InventoryManagerComponent->EquipInventoryWidget->RemoveFromViewport();
}

void APathogenesisCharacter::BeginPlay()
{
	Super::BeginPlay();

	UEquipInventoryComponent* EquipInv = Cast<UEquipInventoryComponent>(EquipmentInventoryComponent);
	EquipInv->OnItemEquipped.AddUObject(this, &APathogenesisCharacter::EquipItem);
	EquipInv->OnItemUnequipped.AddUObject(this, &APathogenesisCharacter::UnequipItem);
}

void APathogenesisCharacter::EquipItem(FInventorySlotInfo ItemInfo)
{
	if (ItemInfo.ItemData)
	{
		if (ItemInfo.ItemData->EquipSlot == EEquipSlot::ES_Primary)															//Equip item to slot. Check for primary weapon
		{
			if (CurrentWeapon && CurrentWeapon->ItemData->EquipSlot == EEquipSlot::ES_Primary)
			{
				FDetachmentTransformRules Rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);

				CurrentWeapon->DetachFromActor(Rules);
				CurrentWeapon->SetActorLocation(FVector(0, 0, -100));
				CurrentWeapon->SetActorHiddenInGame(true);
				CurrentWeapon = nullptr;
			}

			FName PrimarySocketName = TEXT("PrimarySocket");
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);


			AWeaponActor* EquipActor = Cast<AWeaponActor>(ItemInfo.ItemActor);

			if (EquipActor)
			{
				Rules.LocationRule = EAttachmentRule::SnapToTarget;
				Rules.RotationRule = EAttachmentRule::SnapToTarget;
				Rules.ScaleRule = EAttachmentRule::KeepRelative;

				EquipActor->ItemData = ItemInfo.ItemData;
				EquipActor->AttachToComponent(this->GetMesh(), Rules, PrimarySocketName);
				EquipActor->SetActorHiddenInGame(false);
			}

			PrimarySocket = EquipActor;
		}
		else if (ItemInfo.ItemData->EquipSlot == EEquipSlot::ES_Secondary)															//Equip item to slot. Check for secondary weapon
		{

			if (CurrentWeapon && CurrentWeapon->ItemData->EquipSlot == EEquipSlot::ES_Secondary)
			{
				FDetachmentTransformRules Rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);

				CurrentWeapon->DetachFromActor(Rules);
				CurrentWeapon->SetActorLocation(FVector(0, 0, -100));
				CurrentWeapon->SetActorHiddenInGame(true);
				CurrentWeapon = nullptr;
			}

			FName SecondarySocketName = TEXT("SecondarySocket");
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);


			AWeaponActor* EquipActor = Cast<AWeaponActor>(ItemInfo.ItemActor);

			if (EquipActor)
			{
				Rules.LocationRule = EAttachmentRule::SnapToTarget;
				Rules.RotationRule = EAttachmentRule::SnapToTarget;
				Rules.ScaleRule = EAttachmentRule::KeepRelative;

				EquipActor->ItemData = ItemInfo.ItemData;
				EquipActor->AttachToComponent(this->GetMesh(), Rules, SecondarySocketName);
				EquipActor->SetActorHiddenInGame(false);
			}

			SecondarySocket = EquipActor;
		}

		if (ItemInfo.ItemData->EquipSlot == EEquipSlot::ES_Vest && Cast<UMag>(ItemInfo.ItemData))							//Updating ammo if mag added to chest
		{
			CheckAmmo();
		}


		ItemInfo.ItemActor->bIsEquipped = true;
	}
}

void APathogenesisCharacter::UnequipItem(FInventorySlotInfo ItemInfo)										//Clearing old delegates when current weapon changed
{

	if (ItemInfo.ItemData)
	{
		if (ItemInfo.ItemData->EquipSlot == EEquipSlot::ES_Primary)
		{
			PrimarySocket = nullptr;

			if (CurrentWeapon == ItemInfo.ItemActor)
			{
				CurrentWeapon->OnMagLoaded.Clear();
				CurrentWeapon->OnBulletLoaded.Clear();
				CurrentWeapon->OnAmmoChanged.Clear();

				CurrentWeapon = nullptr;
				OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
			}


			FDetachmentTransformRules Rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);

			ItemInfo.ItemActor->DetachFromActor(Rules);
			ItemInfo.ItemActor->SetActorLocation(FVector(0, 0, -100));
			ItemInfo.ItemActor->SetActorHiddenInGame(true);

			CheckAmmo();
		}

		else if (ItemInfo.ItemData->EquipSlot == EEquipSlot::ES_Vest && Cast<UMag>(ItemInfo.ItemData))
		{
			CheckAmmo();
		}

		ItemInfo.ItemActor->bIsEquipped = false;
	}
}

void APathogenesisCharacter::EquipPrimaryWeapon()
{
	if (PrimarySocket && PrimarySocket != CurrentWeapon)
	{
		if (CurrentWeapon)
		{
			FName SecondarySocketName = TEXT("SecondarySocket");
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

			Rules.LocationRule = EAttachmentRule::SnapToTarget;
			Rules.RotationRule = EAttachmentRule::SnapToTarget;
			Rules.ScaleRule = EAttachmentRule::KeepRelative;

			CurrentWeapon->AttachToComponent(this->GetMesh(), Rules, SecondarySocketName);
		}

		CurrentWeapon = PrimarySocket;
		FName WeaponSocketName = TEXT("hand_rSocket");
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

		Rules.LocationRule = EAttachmentRule::SnapToTarget;
		Rules.RotationRule = EAttachmentRule::SnapToTarget;
		Rules.ScaleRule = EAttachmentRule::KeepRelative;

		CurrentWeapon->AttachToComponent(this->GetMesh(), Rules, WeaponSocketName);
		if (OnCurrentWeaponChanged.IsBound())
		{
			OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
		}
		CheckAmmo();

	}
}

void APathogenesisCharacter::EquipSecondaryWeapon()
{

	if (SecondarySocket && SecondarySocket != CurrentWeapon)
	{
		if (CurrentWeapon)
		{
			FName PrimarySocketName = TEXT("PrimarySocket");
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

			Rules.LocationRule = EAttachmentRule::SnapToTarget;
			Rules.RotationRule = EAttachmentRule::SnapToTarget;
			Rules.ScaleRule = EAttachmentRule::KeepRelative;

			CurrentWeapon->AttachToComponent(this->GetMesh(), Rules, PrimarySocketName);
		}

		CurrentWeapon = SecondarySocket;
		FName WeaponSocketName = TEXT("hand_rSocket");
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

		Rules.LocationRule = EAttachmentRule::SnapToTarget;
		Rules.RotationRule = EAttachmentRule::SnapToTarget;
		Rules.ScaleRule = EAttachmentRule::KeepRelative;

		CurrentWeapon->AttachToComponent(this->GetMesh(), Rules, WeaponSocketName);
		if (OnCurrentWeaponChanged.IsBound())
		{
			OnCurrentWeaponChanged.Broadcast(CurrentWeapon);
		}
		CheckAmmo();

	}
 
}

bool APathogenesisCharacter::Reload()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Reload")), true);
	if (CurrentWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Casted")), true);

		int32 Count = 0;
		int32 MagIndex = -1;
		AMagActor* MagToLoad = nullptr;

		for (auto& item : EquipmentInventoryComponent->GetItems())																	//Searching for mag with maximum bullets
		{

			AMagActor* MagActor = Cast<AMagActor>(item.Value.ItemActor);

			if (MagActor)
			{
				if (CurrentWeapon->AmmoType == MagActor->AmmoType && MagActor->Bullets.Num() > Count)
				{
					Count = MagActor->Bullets.Num();
					MagIndex = item.Key;
					MagToLoad = MagActor;
				}
			}
		}

		if (MagToLoad)
		{
			if (!CurrentWeapon->MagActor)
			{
				CurrentWeapon->MagActor = MagToLoad;
				EquipmentInventoryComponent->ClearItem(MagIndex);
				InventoryManagerComponent->ReinitInventoryWidgets(InventoryComponent, EquipmentInventoryComponent);
				CurrentWeapon->OnMagLoaded.Broadcast(MagToLoad);
				CheckAmmo();
			}
			else
			{
				AMagActor* temp = CurrentWeapon->MagActor;
				CurrentWeapon->MagActor = MagToLoad;
				EquipmentInventoryComponent->SetItem(MagIndex, FInventorySlotInfo{ temp->ItemData, temp });
				CurrentWeapon->OnMagLoaded.Broadcast(MagToLoad);
				CheckAmmo();
			}

			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Mag %i"), MagToLoad->Bullets.Num()), true);
			return true;
		}
	}

		return false;
}

bool APathogenesisCharacter::PickUpItem(class APickableActor* Actor)
{
	Actor->SetActorEnableCollision(false);

	UItem* ItemData = Actor->ItemData;

	if (ItemData->MaxCount > 1)
	{
		for (auto& item : InventoryComponent->GetItems())														//Check for addable items
		{
			if (item.Value.ItemData == ItemData)
			{
				item.Value.ItemActor->Count += Actor->Count;
				Actor->Destroy();
				return true;
			}
		}
	}

	for (int32 CurrentSlotIndex = 0; CurrentSlotIndex < 30; CurrentSlotIndex++)														//Search slot position for unique items
	{
		if (!InventoryComponent->GetItems().Contains(CurrentSlotIndex))
		{
			FInventorySlotInfo AddItem;
			AddItem.ItemData = ItemData;
			AddItem.ItemActor = Actor;
			AddItem.ItemActor->Count = Actor->Count;

			InventoryComponent->SetItem(CurrentSlotIndex, AddItem);
			Actor->SetActorLocation(FVector(0, 0, -100));
			Actor->SetActorHiddenInGame(true);
			Actor->bIsPicked = true;

			return true;
		}
	}


	Actor->SetActorEnableCollision(true);
	return false;
}

void APathogenesisCharacter::CheckAmmo()
{
	int32 Count = 0;

	if (CurrentWeapon)
	{
		for (auto& item : EquipmentInventoryComponent->GetItems())
		{
			AMagActor* MagActor = Cast<AMagActor>(item.Value.ItemActor);
			if (MagActor && MagActor->AmmoType == CurrentWeapon->AmmoType)
			{
				Count += MagActor->Bullets.Num();
			}
		}

		if (CurrentWeapon->OnAmmoChanged.IsBound())
		{
			CurrentWeapon->OnAmmoChanged.Broadcast(Count);
		}
	}
}

void APathogenesisCharacter::UseMedKit()
{
	auto& items = EquipmentInventoryComponent->GetItems();
	APathogenesisMedKit* MedKit = Cast<APathogenesisMedKit>(items.Find(7)->ItemActor);
	if (MedKit)
	{
		HealthManager->AddHealth(MedKit->Power);
		HealthManager->OnDamageTaken.Broadcast(HealthManager->GetHealthPercentage());
		EquipmentInventoryComponent->ClearItem(7);
	}
	
}

void APathogenesisCharacter::ReinitWidgets()
{
	InventoryManagerComponent->ReinitInventoryWidgets(InventoryComponent, EquipmentInventoryComponent);
}

