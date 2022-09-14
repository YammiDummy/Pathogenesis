// Fill out your copyright notice in the Description page of Project Settings.


#include "PathogenesisMedKit.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
APathogenesisMedKit::APathogenesisMedKit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MedKitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MedkitMesh"));
	MedKitMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APathogenesisMedKit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathogenesisMedKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}