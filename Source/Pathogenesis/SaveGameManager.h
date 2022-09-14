// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveGameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFromSlotAction, const FString&, SlotName);

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API USaveGameManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize();

	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentGame(const FString& SlotName);

	UPROPERTY(BlueprintAssignable)
	FOnGameFromSlotAction OnGameSaved;

	UPROPERTY(BlueprintAssignable)
	FOnGameFromSlotAction OnGameLoaded;

protected:

	void OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, class USaveGame* SaveGame);
	void OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	UPROPERTY(BlueprintReadWrite)
	class UPathogenesisSaveGame* CurrentGameObject;
};
