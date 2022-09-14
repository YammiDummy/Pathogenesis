// Fill out your copyright notice in the Description page of Project Settings.


#include "MagActor.h"
#include <Components/StaticMeshComponent.h>

AMagActor::AMagActor()
{
	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMesh"));
	MagMesh->SetupAttachment(RootComponent);
}
