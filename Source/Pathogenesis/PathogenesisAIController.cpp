// Fill out your copyright notice in the Description page of Project Settings.


#include "PathogenesisAIController.h"

APathogenesisAIController::APathogenesisAIController()
{
	TeamID = FGenericTeamId(1);
}

ETeamAttitude::Type APathogenesisAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	auto OtherPawn = Cast<APawn>(&Other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	auto Bot = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	auto Player = Cast<IGenericTeamAgentInterface>(&Other);

	if (Bot == nullptr && Player == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId OtherActorTeamId = NULL;
	if (Bot)
	{
		OtherActorTeamId = Bot->GetGenericTeamId();
	}
	else if (Player)
	{
		OtherActorTeamId = Player->GetGenericTeamId();
	}

	FGenericTeamId ThisBotTeamId = this->GetGenericTeamId();
	if (OtherActorTeamId == 255)
	{
		return ETeamAttitude::Neutral;
	}
	else if (ThisBotTeamId == OtherActorTeamId)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}
}

ETeamAttitude::Type APathogenesisAIController::GetAttitudeTowards(const AActor* Other) const
{
	return GetTeamAttitudeTowards(*Other);
}
