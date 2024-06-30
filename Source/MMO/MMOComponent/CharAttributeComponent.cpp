// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOComponent/CharAttributeComponent.h"

// Sets default values for this component's properties
UCharAttributeComponent::UCharAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharAttributeComponent::Init(int32 health, FString charName, int32 level)
{
	this->Health = health;
	this->MaxHealth = health;
	this->CharName = charName;
	this->Level = level;
}

bool UCharAttributeComponent::IsAlive()
{
	if (Health > 0)
	{
		return true;
	}

	return false;
}

float UCharAttributeComponent::GetHelathPercent()
{
	if (MaxHealth > 0)
	{
		return Health / (float)MaxHealth;
	}
	return 0.0f;
}

void UCharAttributeComponent::GetDamage(int damage)
{
	Health -= damage;
	if (Health < 0)
	{
		Health = 0;
	}
}

void UCharAttributeComponent::RecoverHealth(int recover)
{
	Health += recover;
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
}

