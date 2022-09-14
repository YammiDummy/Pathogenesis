// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathogenesisGameMode.h"
#include "PathogenesisPlayerController.h"
#include "PathogenesisCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Engine/AssetManager.h>

APathogenesisGameMode::APathogenesisGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APathogenesisPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//UAssetManager& AssetManager = UAssetManager::Get();
	//AssetManager.GetPrimaryAssetIdList()
}