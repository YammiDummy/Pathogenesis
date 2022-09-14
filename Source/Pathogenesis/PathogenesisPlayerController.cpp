// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathogenesisPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "PathogenesisCharacter.h"
#include "Engine/World.h"
#include <Tasks/AITask_MoveTo.h>
#include <NavigationSystem.h>
#include "InventoryManagerComponent.h"

APathogenesisPlayerController::APathogenesisPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void APathogenesisPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (GetPawn()->GetVelocity().Size() > 0)
	{
		return;
	}
	else if (PathVectors.Num() > 0)
	{
		float DistanceToPoint = FVector::Distance(GetPawn()->GetActorLocation(), PathVectors[0]);
		if (DistanceToPoint > 120.f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, PathVectors[0]);
		}
		else if (DistanceToPoint < 120.f)
		{
			PathVectors.RemoveAt(0, 1, true);
		}
	}

}

void APathogenesisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APathogenesisPlayerController::OnSetDestinationPressed);

	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APathogenesisPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APathogenesisPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &APathogenesisPlayerController::OnResetVR);
}

void APathogenesisPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APathogenesisPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (APathogenesisCharacter* MyPawn = Cast<APathogenesisCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void APathogenesisPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APathogenesisPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APathogenesisPlayerController::OnSetDestinationPressed()
{

	FHitResult Hit;
	
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{

		if (Hit.Component->GetCollisionObjectType() == ECC_GameTraceChannel2)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
		}
	}
}

void APathogenesisPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void APathogenesisPlayerController::Stop()
{
	PathVectors.Empty();
	StopMovement();
}
