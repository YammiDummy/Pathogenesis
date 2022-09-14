// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PathogenesisGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PATHOGENESIS_API UPathogenesisGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
	class USaveGameManager* SaveManager;
};
