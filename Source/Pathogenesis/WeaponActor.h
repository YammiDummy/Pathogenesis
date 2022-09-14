// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "MagActor.h"
#include "WeaponActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagLoaded, class AMagActor*, NewMag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletLoaded, AWeaponActor*, WeaponName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, AmmoNum);


UCLASS()
class PATHOGENESIS_API AWeaponActor : public APickableActor
{
	GENERATED_BODY()
	
public:


	UPROPERTY(BlueprintAssignable)
	FOnMagLoaded OnMagLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnBulletLoaded OnBulletLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTimerHandle FireHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EAmmoType AmmoType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsChamberLoaded;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsNoAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsJammed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsFullAuto;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAmmo* CurrentBullet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AMagActor* MagActor;

	UFUNCTION(BlueprintCallable)
	void LoadBullet();

};
