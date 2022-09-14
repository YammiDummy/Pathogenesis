// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameManager.h"
#include <Kismet/GameplayStatics.h>
#include "PathogenesisSaveGame.h"

void USaveGameManager::Initialize()
{
	CurrentGameObject = Cast<UPathogenesisSaveGame>(UGameplayStatics::CreateSaveGameObject(UPathogenesisSaveGame::StaticClass()));

}

bool USaveGameManager::DoesSaveGameExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveGameManager::LoadGame(const FString& SlotName)
{
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &USaveGameManager::OnGameLoadedFromSlotHandle));
}

void USaveGameManager::SaveCurrentGame(const FString& SlotName)
{
	UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USaveGameManager::OnGameSavedToSlotHandle));
}

void USaveGameManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, class USaveGame* SaveGame)
{
	CurrentGameObject = Cast<UPathogenesisSaveGame>(SaveGame);
	if (OnGameLoaded.IsBound())
	{
		OnGameLoaded.Broadcast(SlotName);
	}
}

void USaveGameManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (OnGameSaved.IsBound())
	{
		OnGameSaved.Broadcast(SlotName);
	}
}
