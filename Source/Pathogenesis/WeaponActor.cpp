// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

void AWeaponActor::LoadBullet()
{
	if (MagActor)
	{
		int32 BulletsCount = MagActor->Bullets.Num();

		if (BulletsCount > 0)
		{
			CurrentBullet = MagActor->Bullets.Pop(true);
			bIsChamberLoaded = true;
			
			if (OnBulletLoaded.IsBound())
			{
				OnBulletLoaded.Broadcast(this);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Mag is empty")), true);
		}
	}
}
