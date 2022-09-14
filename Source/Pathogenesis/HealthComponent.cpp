// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::AddHealth(int32 Health)
{
	CurrentHealth += Health;
	if (CurrentHealth > MaximumHealth)
	{
		CurrentHealth = MaximumHealth;
	}
}

float UHealthComponent::GetHealthPercentage()
{
	return CurrentHealth / MaximumHealth;
}

void UHealthComponent::TakeDamage(float Damage)
{
	if (CurrentHealth > 0)
	{
		if (CurrentHealth - Damage > 0)
		{
			CurrentHealth -= Damage;
			if (OnDamageTaken.IsBound())
			{
				OnDamageTaken.Broadcast(GetHealthPercentage());
			}
		}
		else
		{
			CurrentHealth = 0;
			if (OnDamageTaken.IsBound())
			{
				OnDamageTaken.Broadcast(GetHealthPercentage());
			}

			if (OnDie.IsBound())
			{
				OnDie.Broadcast();
			}
		}
	}
}

