// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PathogenesisAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PATHOGENESIS_API APathogenesisAIController : public AAIController
{
	GENERATED_BODY()
public:
	APathogenesisAIController();
	
private:

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGenericTeamId TeamID;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	ETeamAttitude::Type GetAttitudeTowards(const AActor* Other) const;
};
