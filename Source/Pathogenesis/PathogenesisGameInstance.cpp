// Fill out your copyright notice in the Description page of Project Settings.


#include "PathogenesisGameInstance.h"
#include "SaveGameManager.h"

void UPathogenesisGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Init"));

	SaveManager = NewObject<USaveGameManager>(this);
	SaveManager->Initialize();
}
