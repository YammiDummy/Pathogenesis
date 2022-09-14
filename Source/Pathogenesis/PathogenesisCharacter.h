// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DataAssets/Item.h"
#include "GenericTeamAgentInterface.h"
#include "WeaponActor.h"
#include "PathogenesisCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, class AActor*, WeaponActor);

UCLASS(Blueprintable)
class APathogenesisCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APathogenesisCharacter();

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")		
	class UHealthComponent* HealthManager;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UChildActorComponent* CurrentWeapon;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	FGenericTeamId TeamId;

public:

	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void CloseInventory();

	UPROPERTY(BlueprintAssignable)
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UInventoryComponent* EquipmentInventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeaponActor* CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeaponActor* PrimarySocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeaponActor* SecondarySocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* HeadSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* BodySocket;

	UFUNCTION(BlueprintCallable)
	void EquipItem(FInventorySlotInfo ItemInfo);

	UFUNCTION(BlueprintCallable)
	void UnequipItem(FInventorySlotInfo ItemInfo);

	UFUNCTION(BlueprintCallable)
	void EquipPrimaryWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipSecondaryWeapon();

	UFUNCTION(BlueprintCallable)
	bool Reload();

	UFUNCTION(BlueprintCallable)
	bool PickUpItem(class APickableActor* Actor);

	UFUNCTION(BlueprintCallable)
	void CheckAmmo();

	UFUNCTION(BlueprintCallable)
	void UseMedKit();

	void ReinitWidgets();
};