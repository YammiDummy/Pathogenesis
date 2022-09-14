// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "PathogenesisMedKit.generated.h"

UCLASS()
class PATHOGENESIS_API APathogenesisMedKit : public APickableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathogenesisMedKit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* MedKitMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;
};
