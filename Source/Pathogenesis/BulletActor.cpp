// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include <Components/StaticMeshComponent.h>

ABulletActor::ABulletActor()
{
	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMeshComponent;
}
