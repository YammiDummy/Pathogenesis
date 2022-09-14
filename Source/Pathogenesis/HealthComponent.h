// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PATHOGENESIS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable)
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable)
	FOnDie OnDie;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaximumHealth;

	UFUNCTION(BlueprintCallable)
	void AddHealth(int32 Health);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);
};
