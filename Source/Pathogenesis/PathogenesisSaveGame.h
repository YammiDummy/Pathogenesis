// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PickableActor.h"
#include "MagActor.h"
#include "PathogenesisSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;
};

USTRUCT(BlueprintType)
struct FBotState
{
	GENERATED_BODY()

public:



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ACharacter> CharClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BotLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health;
};

USTRUCT(BlueprintType)
struct FPickableStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<APickableActor> PickableClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform ActorTransfom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DataAssetName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Count;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPicked;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsEquipped;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Index;
};


USTRUCT(BlueprintType)
struct FMagStruct : public FPickableStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> BulletsArray;
};

USTRUCT(BlueprintType)
struct FPickableWeapon : public FPickableStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FMagStruct MagStruct;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsWithMag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsCurrent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsChamberLoaded;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsJammed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString CurrentBullet;
};



UCLASS()
class PATHOGENESIS_API UPathogenesisSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString MapName;

	UPROPERTY(BlueprintReadWrite, Savegame)
	FCharacterState PlayerState;

	UPROPERTY(BlueprintReadWrite, Savegame)
	TArray<FBotState> BotActors;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FPickableStruct> PickableActors;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FPickableWeapon> PickableWeapons;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FMagStruct> PickableMags;
};
